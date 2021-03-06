/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published by
 * the Free Software Foundation.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "viewport.hpp"
#define GLM_FORCE_RADIANS
#include <glm/gtx/vector_angle.hpp>

namespace como {


GLint Viewport::viewProjectionMatrixLocation = -1;

const float ORTHO_CUBE_SIDE = 2.0f;
const float Z_NEAR = 0.01f;
const float Z_FAR = 100.0f;


/***
 * 1. Construction
 ***/

Viewport::Viewport( View view, Projection projection, std::shared_ptr< ComoApp > comoApp ) :
    QWindow(),
    comoApp( comoApp ),
    localEntitiesSelection_( comoApp->getScene()->getEntitiesManager()->getLocalSelection() ),
    camera( nullptr ),
    widthInverse( 0.0f ),
    heightInverse( 0.0f ),
    projectionMatrix( 1.0f ),
    view_( View::FRONT ),
    projection_( projection ),
    forceRender_( true ),
    lastMouseWorldPos_( 0.0f ),
    camerasManager_( comoApp->getScene()->getEntitiesManager()->getCamerasManager() )
{
    try {
        OpenGL::checkStatus( "Viewport constructor - begin" );
        GLint currentShaderProgram;

        // Make this canvas share the given app's state.
        this->comoApp = comoApp;

        // We will render using OpenGL.
        setSurfaceType( QWindow::OpenGLSurface );

        // Set this surface to the same format used by the app's shared OpenGL context.
        setFormat( comoApp->getScene()->getOpenGLContext()->format() );
        destroy();
        create();

        assert( comoApp->getScene()->getOpenGLContext()->makeCurrent( this ) );

        // Get location of uniform shader modelview matrix.
        if( viewProjectionMatrixLocation == -1 ){
            // Get current shader program id.
            glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
            assert( currentShaderProgram != 0 );

            viewProjectionMatrixLocation =
                    comoApp->getScene()->getOpenGL()->getShaderVariableLocation( "mvpMatrix" );
        }
        assert( viewProjectionMatrixLocation != -1 );

        // Create the camera.
        camera = new Camera( NO_RESOURCE, *( comoApp->getScene()->getOpenGL() ) );

        // Set the requested view.
        setView( view );

        // Set the request projection
        setProjection( projection );

        OpenGL::checkStatus( "Viewport constructor - end" );
    }catch( std::exception& ex ){
        comoApp->getLog()->error( ex.what(), "\n" );
        throw;
    }
}


/***
 * 2. Destruction
 ***/

Viewport::~Viewport()
{
    delete camera;

    comoApp->getLog()->debug( "Viewport destroyed\n" );
}


/***
 * 3. Events
 ***/

bool Viewport::event(QEvent *event)
{
    switch (event->type()) {
        case QEvent::UpdateRequest:
            // Update requested. Render the surface.
            render();

            // Returning true prevents the event from being sent on to other objects.
            return true;
        break;
        default:
            return QWindow::event(event);
        break;
    }
}


void Viewport::exposeEvent( QExposeEvent *event )
{
    // We are not interested in using the argument "event". The following macro prevents the
    // compiler from giving us a warning.
    Q_UNUSED(event);

    // Render this viewport if it's exposed.
    if ( isExposed() ){
        render();
    }
}


void Viewport::resizeEvent(QResizeEvent *event)
{
    // We are not interested in using the argument "event". The following macro prevents the
    // compiler from giving us a warning.
    Q_UNUSED(event);

    // Recompute the inverses of canvas' dimensions.
    if( width() && height() ){
        widthInverse = 1.0f / width();
        heightInverse = 1.0f / height();

        // Projection matrix depends on viewport dimensions, so update it.
        setProjection( projection_ );
    }
}


void Viewport::mousePressEvent( QMouseEvent* mousePressEvent )
{
    glm::vec3 rayOrigin, rayDirection;
    float t;
    bool addToSelection = false;

    // Do one thing or another depending on which transformation mode we are in.
    if( comoApp->getTransformationType() == TransformationType::NONE ){
        // We are not in transformation mode. This mouse press is for selecting
        // a drawable.

        // http://en.wikibooks.org/wiki/OpenGL_Programming/Object_selection
        // Tace a ray from pixel towards camera's center vector.
        traceRay( mousePressEvent->x(), height() - mousePressEvent->y() - 1, rayOrigin, rayDirection );

        // If user is pressing ctrl key while selecting a drawable, add it to the current
        // selection. Otherwise, clear the current selection and select the new drawable.
        addToSelection = mousePressEvent->modifiers() & Qt::ControlModifier;

        // Do the ray picking.
        comoApp->getScene()->getEntitiesManager()->selectEntityByRayPicking( rayOrigin,
                                                         rayDirection,
                                                         addToSelection,
                                                         t );

        lastMouseWorldPos_ = rayOrigin + rayDirection * t;
    }else{
        // We were in transformation mode. This mouse press is for droping
        // the current selection.
        comoApp->setTransformationType( TransformationType::NONE );
    }
}


void Viewport::wheelEvent( QWheelEvent *ev )
{
    // TODO: If we are in View::CAMERA, change to View::USER and apply zoom.
    if( view_ != View::CAMERA ){
        const float step = ( ev->delta() > 0 ) ? 0.1f : -0.1f;
        const glm::vec3 direction = step * glm::vec3( camera->getCenterVector() );

        camera->translate( direction );

        forceRender();
    }
}


void Viewport::keyPressEvent( QKeyEvent *e )
{
    switch ( e->key() )
    {
        case Qt::Key_T:
            comoApp->setTransformationType( TransformationType::TRANSLATION );
            comoApp->setTransformationMode( TransformationMode::FREE );
        break;
        case Qt::Key_R:
            comoApp->setTransformationType( TransformationType::ROTATION );
            comoApp->setTransformationMode( TransformationMode::FREE );
        break;
        case Qt::Key_S:
            comoApp->setTransformationType( TransformationType::SCALE );
            comoApp->setTransformationMode( TransformationMode::FREE );
        break;
        case Qt::Key_X:
            comoApp->setTransformationMode( TransformationMode::FIXED_X );
        break;
        case Qt::Key_Y:
            comoApp->setTransformationMode( TransformationMode::FIXED_Y );
        break;
        case Qt::Key_Z:
            comoApp->setTransformationMode( TransformationMode::FIXED_Z );
        break;
        case Qt::Key_Delete:
            comoApp->getScene()->getEntitiesManager()->requestSelectionDeletion();
        break;
        case Qt::Key_Up:
            if( view_ != View::CAMERA ){
                camera->translate( 0.1f * camera->getUpVector() );
            }
        break;
        case Qt::Key_Down:
            if( view_ != View::CAMERA ){
                camera->translate( -0.1f * camera->getUpVector() );
            }
        break;
        case Qt::Key_Left:
            if( view_ != View::CAMERA ){
                const glm::vec3 direction =
                        0.1f * glm::cross( camera->getUpVector(),
                                            glm::vec3( camera->getCenterVector() ) );

                camera->translate( direction );
            }
        break;
        case Qt::Key_Right:
            if( view_ != View::CAMERA ){
                camera->translate( -0.1f * glm::cross( camera->getUpVector(),
                                                       glm::vec3( camera->getCenterVector() ) ) );
            }
        break;
        default:
        break;
    }
}


void Viewport::mouseMoveEvent( QMouseEvent* mouseMoveEvent )
{   
    // Don't handle this event if no entity is selected.
    if( localEntitiesSelection_->size() == 0 ){
        return;
    }

    // Reference axis used in rotations.
    const glm::vec3 xAxis( 1.0f, 0.0f, 0.0f );
    const glm::vec3 yAxis( 0.0f, 1.0f, 0.0f );
    const glm::vec3 zAxis( 0.0f, 0.0f, 1.0f );

    // Variables used for computing the magnitude of the transformation.
    glm::vec3 transformVector;
    float angle;
    const glm::vec3 scenePivotPoint = localEntitiesSelection_->graphicPivotPoint();
    glm::vec3 rayOrigin, rayDirection;
    traceRay( mouseMoveEvent->x(), height() - mouseMoveEvent->y() - 1, rayOrigin, rayDirection );

    // Compute the coefficients of the plane which contains the point
    // "lastMouseWorldPos_" and whose normal is "-camera->getCenterVector()".
    const float A = -currentCamera().getCenterVector().x;
    const float B = -currentCamera().getCenterVector().y;
    const float C = -currentCamera().getCenterVector().z;
    const float D = -( A * lastMouseWorldPos_.x + B * lastMouseWorldPos_.y + C * lastMouseWorldPos_.z );

    // Get the intersection between the previous ray and plane.
    const float t = -( ( A * rayOrigin.x + B * rayOrigin.y + C * rayOrigin.z + D ) / ( A * rayDirection.x + B * rayDirection.y + C * rayDirection.z ) );
    const glm::vec3 currentMouseWorldPos = rayOrigin + rayDirection * t;

    if( !std::isfinite( D ) || !std::isfinite( t ) ){
        return;
    }

    const glm::vec3 lastMouseWorldRelPos = lastMouseWorldPos_ - scenePivotPoint;
    const glm::vec3 currentMouseWorldRelPos = currentMouseWorldPos - scenePivotPoint;

    // Get current app's transformation type and mode.
    TransformationType transformationType = comoApp->getTransformationType();
    TransformationMode transformationMode = comoApp->getTransformationMode();

    // Make the transformation requested by user.
    switch( transformationType ){
        case TransformationType::TRANSLATION:
            // Compute the transformation vector.
            transformVector = currentMouseWorldPos - lastMouseWorldPos_;

            // If requested, attach the translation vector to an axis.
            switch( transformationMode ){
                case TransformationMode::FIXED_X:
                    transformVector.y = 0.0f;
                    transformVector.z = 0.0f;
                break;
                case TransformationMode::FIXED_Y:
                    transformVector.x = 0.0f;
                    transformVector.z = 0.0f;
                break;
                case TransformationMode::FIXED_Z:
                    transformVector.x = 0.0f;
                    transformVector.y = 0.0f;
                break;
                default:
                break;
            }

            // Do the translation.
            localEntitiesSelection_->translate( glm::vec3( transformVector ) );
        break;
        case TransformationType::ROTATION:{
            if( ( currentMouseWorldRelPos.length() == 0.0f ) ||
                ( lastMouseWorldRelPos.length() == 0.0f ) ){
                break;
            }

            angle = glm::orientedAngle(
                        glm::normalize( currentMouseWorldRelPos ),
                        glm::normalize( lastMouseWorldRelPos ),
                        glm::vec3( currentCamera().getCenterVector() ) );

            // Make the rotation about an axis or another depending on the current
            // transformationMode.
            switch( transformationMode ){
                case TransformationMode::FIXED_X:
                    localEntitiesSelection_->rotate( angle, xAxis );
                break;
                case TransformationMode::FIXED_Y:
                    localEntitiesSelection_->rotate( angle, yAxis );
                break;
                case TransformationMode::FIXED_Z:
                    localEntitiesSelection_->rotate( angle, zAxis );
                break;
                case TransformationMode::FREE:
                    localEntitiesSelection_->rotate( angle, glm::vec3( -currentCamera().getCenterVector() ) );
                break;
            }
        }break;
        case TransformationType::SCALE:{
            // Error checking : if lastMouseWorldRelPos's
            // components would be zero, we would have a NaN when
            // dividing by it.
            if( lastMouseWorldRelPos.x != 0.0f &&
                lastMouseWorldRelPos.y != 0.0f &&
                lastMouseWorldRelPos.z != 0.0f ){
                transformVector = glm::vec3( currentMouseWorldRelPos.x / lastMouseWorldRelPos.x,
                                             currentMouseWorldRelPos.y / lastMouseWorldRelPos.y,
                                             currentMouseWorldRelPos.z / lastMouseWorldRelPos.z );

                if(     !std::isfinite( transformVector.x ) ||
                        !std::isfinite( transformVector.y ) ||
                        !std::isfinite( transformVector.z ) ){
                    break;
                }

                // Compute the difference between mouse's current and last
                // positions.
                glm::vec3 diffVector = currentMouseWorldRelPos - lastMouseWorldRelPos;

                // Get the greatest absolute component from previous
                // difference.
                float scaleFactorIndex;
                if( pow( diffVector.x, 2 ) > pow( diffVector.y, 2 ) ){
                    scaleFactorIndex = 0;
                }else{
                    scaleFactorIndex = 1;
                }
                if( pow( diffVector.z, 2 ) > pow( diffVector[scaleFactorIndex], 2 ) ){
                    scaleFactorIndex = 2;
                }

                // Use previous component as the scale factor. Discard it if
                // it is too small.
                const float scaleFactor =
                        transformVector[scaleFactorIndex];
                if( scaleFactor > -0.01f && scaleFactor < 0.01f ){
                    break;
                }

                // Apply the scale factor to the 3 components.
                transformVector = glm::vec3( scaleFactor );

                // If requested, attach the tranformation vector to an axis.
                switch( transformationMode ){
                    case TransformationMode::FIXED_X:
                        transformVector.y = 1.0f;
                        transformVector.z = 1.0f;
                    break;
                    case TransformationMode::FIXED_Y:
                        transformVector.x = 1.0f;
                        transformVector.z = 1.0f;
                    break;
                    case TransformationMode::FIXED_Z:
                        transformVector.x = 1.0f;
                        transformVector.y = 1.0f;
                    break;
                    default:
                    break;
                }

                // Do the scale only if none of the scaling factors is
                // zero.
                if( transformVector.x != 0.0f &&
                    transformVector.y != 0.0f &&
                    transformVector.z != 0.0f ){
                    // Do the scale.
                    localEntitiesSelection_->scale( glm::vec3( transformVector ) );
                }
            }
        }break;
        default:
        break;
    }

    // Update the transform guide line.
    updateTransformGuideLine( mouseMoveEvent->pos().x(), height() - mouseMoveEvent->pos().y() - 1 );

    // Record last mouse position.
    lastMouseWorldPos_ = currentMouseWorldPos;
}


/***
 * 4. Getters
 ***/

View Viewport::getView() const
{
    return view_;
}

Projection Viewport::getProjection() const
{
    return projection_;
}


/***
 * 5. Updating and drawing
 ***/

void Viewport::renderIfNeeded()
{
    if( forceRender_ ){
        render();
    }
}


void Viewport::render()
{
    // Don't render if surface is not exposed.
    if( !isExposed() ){
        return;
    }

    // Make shared OpenGL context current for this surface.
    comoApp->getScene()->getOpenGLContext()->makeCurrent( this );

    // Make viewport occuppy the full canvas.
    glViewport( 0, 0, width(), height() );

    if( view_ == View::CAMERA && camerasManager_->activeCamera() == nullptr ){
        glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

        // Clear buffers.
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // Swap buffers.
        comoApp->getScene()->getOpenGLContext()->swapBuffers( this );

        return;
    }

    // Clear buffers.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Get camera's view matrix and send it to the shader.
    glm::mat4 viewMatrix = currentCamera().getViewMatrix();
    currentCamera().sendToShader( *( comoApp->getScene()->getOpenGL() ) );

    // Draw scene.
    comoApp->getScene()->draw( viewMatrix, projectionMatrix );

    // If use has selected objects, check if any guide rect must be drawn.
    if( comoApp->getScene()->getEntitiesManager()->getLocalSelection()->size() ){
        // Draw the transformation axis (if requested).
        if( comoApp->getTransformationMode() != TransformationMode::FREE ){
            comoApp->getScene()->linesRenderer()->drawGuideAxis(
                        static_cast< Axis >( static_cast< int >( comoApp->getTransformationMode() ) - 1 ),
                        comoApp->getScene()->getEntitiesManager()->getLocalSelection()->centroid(),
                        viewMatrix,
                        projectionMatrix );
        }

        // Draw guide rect
        if( ( comoApp->getTransformationType() == TransformationType::ROTATION ) ||
            ( comoApp->getTransformationType() == TransformationType::SCALE ) ){
            comoApp->getScene()->linesRenderer()->drawTransformGuideLine( viewMatrix,
                                                                          projectionMatrix );
        }
    }

    // Make viewport occupy the bottom left corner.
    glViewport( width()-50, 0, 50, 50 ); // FIXME: What if width() < 50?

    // Draw scene's world axis.
    comoApp->getScene()->linesRenderer()->drawWorldAxes( viewMatrix );

    // Swap buffers.
    comoApp->getScene()->getOpenGLContext()->swapBuffers( this );

    forceRender_ = false;
}


/***
 * 7. Slots
 ***/

void Viewport::setView( View view )
{
    view_ = view;

    comoApp->getScene()->getOpenGLContext()->makeCurrent( this );

    /***/
    const glm::vec3 center( 0.0f, 0.0f, 0.0f );
    glm::vec3 eye;
    glm::vec3 up;

    switch( view ){
        case View::LEFT:
            eye = glm::vec3( -1.0f, 0.0f, 0.0f );
            up = glm::vec3( 0.0f, 1.0f, 0.0f );
        break;
        case View::RIGHT:
            eye = glm::vec3( 1.0f, 0.0f, 0.0f );
            up = glm::vec3( 0.0f, 1.0f, 0.0f );
        break;
        case View::TOP:
            eye = glm::vec3( 0.0f, 1.0f, 0.0f );
            up = glm::vec3( 0.0f, 0.0f, -1.0f );
        break;
        case View::BOTTOM:
            eye = glm::vec3( 0.0f, -1.0f, 0.0f );
            up = glm::vec3( 0.0f, 0.0f, 1.0f );
        break;
        case View::FRONT:
            eye = glm::vec3( 0.0f, 0.0f, 1.0f );
            up = glm::vec3( 0.0f, 1.0f, 0.0f );
        break;
        case View::BACK:
            eye = glm::vec3( 0.0f, 0.0f, -1.0f );
            up = glm::vec3( 0.0f, 1.0f, 0.0f );
        break;
        case View::USER:
        case View::CAMERA:
            eye = glm::vec3( 1.0f, 1.0f, 1.0f );
            up = glm::vec3( -1.0f, 1.0f, -1.0f );
        break;
    }
    /***/

    camera->setOrientation( eye,
                            center,
                            up );

    // Get the integer index of the current View and return it
    // in a signal.
    emit viewIndexChanged( static_cast< int >( view ) );

    forceRender();
}


void Viewport::setProjection( Projection projection )
{
    projection_ = projection;

    switch( projection_ ){
        case Projection::ORTHO:
            projectionMatrix =
                    glm::ortho(
                        - ORTHO_CUBE_SIDE / 2.0f,
                        ORTHO_CUBE_SIDE / 2.0f,
                        - ORTHO_CUBE_SIDE / 2.0f,
                        ORTHO_CUBE_SIDE / 2.0f,
                        Z_NEAR,
                        Z_FAR );
        break;
        case Projection::PERSPECTIVE:
            projectionMatrix =
                    glm::perspective( glm::pi<float>() / 2.0f,
                                      (float)(width())/(float)(height()),
                                      Z_NEAR,
                                      Z_FAR );
        break;
    }

    forceRender();
}


void Viewport::forceRender()
{
    forceRender_ = true;
}


/***
 * 8. Auxiliar methods
 ***/

glm::vec2 Viewport::getNormalizedMousePos( const int& x, const int& y ) const
{
    return glm::vec2( (x * widthInverse) - 0.5f, (y * heightInverse) - 0.5f );
}


void Viewport::traceRay( const GLfloat& x, const GLfloat& y, glm::vec3& rayOrigin, glm::vec3& rayDirection ) const
{
    glm::vec4 viewport;
    glm::vec3 windowCoordinates;

    // Get this canvas' viewport limits.
    viewport = glm::vec4( 0, 0, width(), height() );

    // Get window coordinates. Set z to near plane's z.
    windowCoordinates = glm::vec3( x, y, 0.0f );

    // Get ray origin coordinates at clipping near plane by unproyecting the window's ones.
    rayOrigin = glm::unProject( windowCoordinates, currentCamera().getViewMatrix(), projectionMatrix, viewport );

    // Get ray direction coordinates by unproyecting the window's ones to far plane and
    // then substracting the ray origin.
    windowCoordinates.z = 1.0f;
    rayDirection = glm::unProject( windowCoordinates, currentCamera().getViewMatrix(), projectionMatrix, viewport ) - rayOrigin;
}


void Viewport::updateTransformGuideLine( const GLfloat& x, const GLfloat& y )
{
    glm::vec3 rayOrigin, rayDestination;
    GLfloat t;
    glm::vec3 destination;

    // The current pivot point will be the rect's origin.
    glm::vec3 origin = localEntitiesSelection_->graphicPivotPoint();

    // Get the equation of a plane which contains the origin point and whose normal
    // is the opposite of the camera's center vector.
    const GLfloat A = ( -currentCamera().getCenterVector() ).x;
    const GLfloat B = ( -currentCamera().getCenterVector() ).y;
    const GLfloat C = ( -currentCamera().getCenterVector() ).z;
    const GLfloat D = -A * origin.x - B * origin.y - C * origin.z;

    // Trace a ray from pixel (x, y) towards camera center vector. Get the the ray's
    // origin (at zNear plane) and direction.
    traceRay( x, y, rayOrigin, rayDestination );

    // Get the intersection between the previous ray and plane.
    t = -( ( A * rayOrigin.x + B * rayOrigin.y + C * rayOrigin.z + D ) / ( A * rayDestination.x + B * rayDestination.y + C * rayDestination.z ) );

    // Use the previous intersection parameter t for computing the guide rect's destination.
    destination = rayOrigin + rayDestination * t;

    // Update the transform guide line with the new origin and destination.
    comoApp->getScene()->linesRenderer()->setTransformGuideLine( origin, destination );
}


const Camera& Viewport::currentCamera() const
{
    if( view_ == View::CAMERA ){
        return *( camerasManager_->activeCamera() );
    }else{
        return *camera;
    }
}

} // namespace como

/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "scene.hpp"

namespace como {

DrawableTypes drawableTypes =
{{
    DrawableType::CUBE
}};

DrawableTypeStrings drawableTypeStrings =
{{
    QString::fromUtf8( "Cube" )
}};

PivotPointModes pivotPointModes =
{{
    PivotPointMode::MEDIAN_POINT,
    PivotPointMode::INDIVIDUAL_CENTROIDS,
    PivotPointMode::WORLD_ORIGIN
}};

// Array with a string for each pivot point mode value (for output in GUI).
PivotPointModeStrings pivotPointModeStrings =
{{
     QString::fromUtf8( "Median Point" ),
     QString::fromUtf8( "Individual Centroid" ),
     QString::fromUtf8( "World origin" )
}};

/***
 * 1. Initialization and destruction
 ***/

Scene::Scene()
{
    GLint currentShaderProgram;
    GLint vPosition;
    GLfloat guideRects[] =
    {
        // X axis
        -100.0f, 0.0f, 0.0f,
        100.0f, 0.0f, 0.0f,

        // Y axis
        0.0f, -100.0f, 0.0f,
        0.0f, 100.0f, 0.0f,

        // Z axis
        0.0f, 0.0f, -100.0f,
        0.0f, 0.0f, 100.0f,

        // Auxiliar vector for rotations and scales
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    };

    // Get the position of the vertex shader variable "vPosition"
    // for the current shader program.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    vPosition = glGetAttribLocation( currentShaderProgram, "vPosition" );
    if( vPosition == GL_INVALID_OPERATION ){
        cout << "Error getting layout of \"position\"" << endl;
    }else{
        cout << "vPosition: (" << vPosition << ")" << endl;
    }
    // Get location of uniform shader variable "color".
    uniformColorLocation = glGetUniformLocation( currentShaderProgram, "color" );
    cout << "Scene: uniform color location initialized to (" << uniformColorLocation << ")" << endl;

    // Set a VBO for the guide rects.
    glGenBuffers( 1, &guideRectsVBO );
    glBindBuffer( GL_ARRAY_BUFFER, guideRectsVBO );
    glBufferData( GL_ARRAY_BUFFER, 24*sizeof( GLfloat ), guideRects, GL_DYNAMIC_DRAW );

    // Set a VAO for the guide rects.
    glGenVertexArrays( 1, &guideRectsVAO );
    glBindVertexArray( guideRectsVAO );

    // By using the previous "vPosition" position, specify the location and data format of
    // the array of vertex positions.
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( vPosition );

    // Initialize world axis
    initWorldAxis();

    // Initializa selection centroid.
    initSelectionCentroid( vPosition );
}


Scene::~Scene()
{
    // Tell OpenGL we are done with allocated buffer objects and
    // vertex attribute arrays.
    glDeleteBuffers( 1, &guideRectsVBO );
    glDeleteVertexArrays( 1, &guideRectsVAO );

    glDeleteBuffers( 1, &selectionCentroidVBO );
    glDeleteVertexArrays( 1, &selectionCentroidVAO );
}


void Scene::initWorldAxis()
{
    GLint currentShaderProgram;
    GLint vPosition;
    const GLfloat x0 = -0.5f;
    const GLfloat y0 = -0.5f;
    const GLfloat z0 = 0.5f;

    const GLfloat x1 = 0.5f;
    const GLfloat y1 = 0.5f;
    const GLfloat z1 = -0.5f;

    GLfloat worldAxisVectors[] =
    {
        // X axis
        x0, y0, z0,
        x1, y0, z0,

        // Y axis
        x0, y0, z0,
        x0, y1, z0,

        // Z axis
        x0, y0, z0,
        x0, y0, z1
    };

    // Get the position of the vertex shader variable "vPosition"
    // for the current shader program.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    vPosition = glGetAttribLocation( currentShaderProgram, "vPosition" );
    if( vPosition == GL_INVALID_OPERATION ){
        cout << "Error getting layout of \"position\"" << endl;
    }else{
        cout << "vPosition: (" << vPosition << ")" << endl;
    }
    // Get location of uniform shader variable "color".
    uniformColorLocation = glGetUniformLocation( currentShaderProgram, "color" );
    cout << "Scene: uniform color location initialized to (" << uniformColorLocation << ")" << endl;

    // Set a VBO for the world axis rects.
    glGenBuffers( 1, &worldAxisVBO );
    glBindBuffer( GL_ARRAY_BUFFER, worldAxisVBO );
    glBufferData( GL_ARRAY_BUFFER, 18*sizeof( GLfloat ), worldAxisVectors, GL_STATIC_DRAW );

    // Set a VAO for the world axis rects.
    glGenVertexArrays( 1, &worldAxisVAO );
    glBindVertexArray( worldAxisVAO );

    // By using the previous "vPosition" position, specify the location and data format of
    // the array of vertex positions.
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( vPosition );
}


void Scene::initSelectionCentroid( const GLuint& vPosition )
{
    GLfloat defaultPivotPoint[3] =
    {
        0.0f, 0.0f, 0.0f
    };

    // Set a VBO for the pivot point.
    glGenBuffers( 1, &selectionCentroidVBO );
    glBindBuffer( GL_ARRAY_BUFFER, selectionCentroidVBO );
    glBufferData( GL_ARRAY_BUFFER, 3*sizeof( GLfloat ), defaultPivotPoint, GL_DYNAMIC_DRAW );

    // Set a VAO for the world axis rects.
    glGenVertexArrays( 1, &selectionCentroidVAO );
    glBindVertexArray( selectionCentroidVAO );

    // By using the previous "vPosition" position, specify the location and data format of
    // the array of vertex positions.
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( vPosition );
}


/***
 * 2. Getters and setters
 ***/

void Scene::setGuideRect( glm::vec3 origin, glm::vec3 destiny )
{
    GLfloat* guideRectsBuffer;
    unsigned int i=0;

    glBindBuffer( GL_ARRAY_BUFFER, guideRectsVBO );
    guideRectsBuffer = (GLfloat *)glMapBufferRange( GL_ARRAY_BUFFER, 18*sizeof( GLfloat ), 6*sizeof( GLfloat ), GL_MAP_WRITE_BIT );

    for( ; i<3; i++ ){
        guideRectsBuffer[i] = origin[i];
    }
    for( ; i<6; i++ ){
        guideRectsBuffer[i] = destiny[i-3];
    }

    glUnmapBuffer( GL_ARRAY_BUFFER );

}


glm::vec3 Scene::getPivotPoint( const PivotPointMode& pivotPointMode )
{
    switch( pivotPointMode ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
        case PivotPointMode::WORLD_ORIGIN:
            return glm::vec3( 0.0f, 0.0f, 0.0f );
        break;
        default:
        //case PivotPointMode::MEDIAN_POINT:
            return glm::vec3( selectionCentroid );
        break;
    }
}


/***
 * 3. Drawables administration
 ***/

void Scene::addDrawable( shared_ptr<Drawable> drawable )
{
    //unselectAll();

    nonSelectedDrawables.push_back( drawable );

    emit renderNeeded();
}

void Scene::addCube( Cube* cube )
{
    DrawablePtr cubePtr( cube );

    addDrawable( cubePtr );
}

void Scene::addDrawable( DrawableType drawableType )
{
    DrawablePtr drawable;
    Cube* cube;

    switch( drawableType ){
        case DrawableType::CUBE:
            cube = new Cube;
            drawable = DrawablePtr( cube );
        break;
    }

    addDrawable( drawable );
}


/***
 * 4. Drawables selection
 ***/

void Scene::selectDrawable( const unsigned int index )
{
    // TODO: In future versions, set a global unique id for each drawables,
    // whether they are selected or not.
    DrawablesList::iterator it = nonSelectedDrawables.begin();
    std::advance( it, index );

    selectedDrawables.splice( selectedDrawables.end(), nonSelectedDrawables, it );

    updateSelectionCentroid();

    emit renderNeeded();
}

void Scene::selectAll()
{
    selectedDrawables.splice( selectedDrawables.end(), nonSelectedDrawables );

    updateSelectionCentroid();

    emit renderNeeded();
}

void Scene::unselectAll()
{
    nonSelectedDrawables.splice( nonSelectedDrawables.end(), selectedDrawables );

    updateSelectionCentroid();

    emit renderNeeded();
}


int Scene::selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection )
{
    const float MAX_T = 9999999.0f;
    float minT = MAX_T;
    float t = MAX_T;
    unsigned int currentObject = 0;
    unsigned int closestObject = -1;

    r1 = glm::normalize( r1 );

    cout << "Scene::selectDrawableByRayPicking" << endl
         << "\tr0 : (" << r0.x << ", " << r0.y << ", " << r0.z << ")" << endl
         << "\tr1 : (" << r1.x << ", " << r1.y << ", " << r1.z << ")" << endl;


    // Does the user want to keep the actual set of selected objects and simply add
    // a new one? If that's NOT the case, we need to clear the set of selected drawables
    // first.
    if( !addToSelection ){
        nonSelectedDrawables.splice( nonSelectedDrawables.end(), selectedDrawables );
    }

    // Iterate over all non selected drawables and check if the given ray intersects
    // them or not. Get the closest object.
    DrawablesList::iterator it;
    for( it = nonSelectedDrawables.begin(); it != nonSelectedDrawables.end(); it++, currentObject++ ){
        (*it)->intersects( r0, r1, t );
        if( ( t >= 0.0f ) && (t < minT ) ){
            // New closest object, get its index and distance.
            closestObject = currentObject;
            minT = t;
        }
    }

    // Iterate over all non selected drawables and check if the given ray intersects
    // them or not. Get the closest object.
    for( it = selectedDrawables.begin(); it != selectedDrawables.end(); it++, currentObject++ ){
        (*it)->intersects( r0, r1, t );
        if( ( t > 0.0f ) && (t < minT ) ){
            // New closest object, get its index and distance.
            closestObject = currentObject;
            minT = t;
            cout << "RETURN 0" << endl;
            emit renderNeeded();
            return 0;
        }
    }

    // If there were intersections, select the closest one.
    if( minT < MAX_T ){
        cout << "FINAL CLOSEST OBJECT: " << closestObject << endl
             << "\t min t: " << minT << ")" << endl
             << "\t min distance: " << glm::distance( glm::vec3( 0.0f, 0.0f, 0.0f ), r1 * t ) << endl;
        selectDrawable( closestObject );
    }else{
        cout << "NO CLOSEST OBJECT. Unselecting all" << endl;
        unselectAll();
    }

    emit renderNeeded();

    return closestObject;
}


glm::vec4 Scene::getSelectionCentroid() const
{
    return selectionCentroid;
}


/***
 * 5. Transformations
 ***/

void Scene::translateSelection( const glm::vec3& direction )
{
    DrawablesList::iterator it = selectedDrawables.begin();

    for( ; it != selectedDrawables.end(); it++ )
    {
        (*it)->translate( direction );
    }

    updateSelectionCentroid();

    emit renderNeeded();
}


void Scene::rotateSelection( const GLfloat& angle, const glm::vec3& axis, const PivotPointMode& pivotPointMode )
{
    DrawablesList::iterator it = selectedDrawables.begin();

    switch( pivotPointMode ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
            for( ; it != selectedDrawables.end(); it++ ){
                (*it)->rotate( angle, axis, glm::vec3( (*it)->getCentroid() ) );
            }
        break;
        case PivotPointMode::MEDIAN_POINT:
            for( ; it != selectedDrawables.end(); it++ ){
                (*it)->rotate( angle, axis, glm::vec3( selectionCentroid ) );
            }
        break;
        case PivotPointMode::WORLD_ORIGIN:
            for( ; it != selectedDrawables.end(); it++ ){
                (*it)->rotate( angle, axis );
            }
        break;
    }

    updateSelectionCentroid();

    emit renderNeeded();
}


void Scene::scaleSelection( const glm::vec3& scaleFactors )
{
    DrawablesList::iterator it = selectedDrawables.begin();

    for( ; it != selectedDrawables.end(); it++ )
    {
        (*it)->scale( scaleFactors );
    }

    updateSelectionCentroid();

    emit renderNeeded();
}

/*
void Scene::rotateSelection( const GLfloat& angle, const glm::vec3& axis, const glm::vec3& pivot )
{
    DrawablesList::iterator it = selectedDrawables.begin();

    for( ; it != selectedDrawables.end(); it++ )
    {
        (*it)->rotate( angle, axis, pivot );
    }
    emit renderNeeded();
}
*/
void Scene::deleteSelection()
{
    // Delete selected drawables.
    selectedDrawables.clear();

    emit renderNeeded();
}


/***
 * 6. Updating
 ***/

void Scene::updateSelectionCentroid()
{
    DrawablesList::const_iterator it = selectedDrawables.begin();
    GLfloat* selectionCentroidBuffer = nullptr;
    selectionCentroid = glm::vec4( 0.0f );

    // Update scene selection centroid.
    if( selectedDrawables.size() ){
        for( ; it != selectedDrawables.end(); it++ ){
            selectionCentroid += (*it)->getCentroid();
        }
        selectionCentroid /= selectedDrawables.size();
        selectionCentroid.w = 1.0f;
    }

    // Map the pivot point VBO to client memory and update the selection centroid
    // coordinates (for drawing).
    glBindBuffer( GL_ARRAY_BUFFER, selectionCentroidVBO );
    selectionCentroidBuffer = (GLfloat *)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );

    for( unsigned int i=0; i<3; i++ ){
        selectionCentroidBuffer[i] = selectionCentroid[i];
    }

    glUnmapBuffer( GL_ARRAY_BUFFER );
}


/***
 * 7. Drawing
 ***/

void Scene::draw( const int& drawGuideRect ) const
{
    GLfloat WHITE_COLOR[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    DrawablesList::const_iterator it = nonSelectedDrawables.begin();

    for( ; it != nonSelectedDrawables.end(); it++ )
    {
        (*it)->draw();
    }
    for( it = selectedDrawables.begin(); it != selectedDrawables.end(); it++ )
    {
        (*it)->draw( true );
    }

    // Draw a guide rect if asked.
    if( drawGuideRect != -1 ){
        // Change painting color to white.
        glUniform4fv( uniformColorLocation, 1, WHITE_COLOR );

        // Bind guide rects' VAO and VBO as the active ones.
        glBindVertexArray( guideRectsVAO );
        glBindBuffer( GL_ARRAY_BUFFER, guideRectsVBO );

        // Draw the guide rect.
        glDrawArrays( GL_LINES, drawGuideRect << 1, 2 );
    }

    // Draw selectionCentroid.
    glDisable( GL_DEPTH_TEST );
    drawSelectionCentroid();
    glEnable( GL_DEPTH_TEST );
}


void Scene::drawWorldAxis() const
{
    GLfloat worldAxisColors[3][4] =
    {
        { 1.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f, 1.0f }
    };

    // Bind world axis rects' VAO and VBO as the active ones.
    glBindVertexArray( worldAxisVAO );
    glBindBuffer( GL_ARRAY_BUFFER, worldAxisVBO );

    // Draw each world axis with its corresponding color.
    for( unsigned int i=0; i<3; i++ ){
        glUniform4fv( uniformColorLocation, 1, worldAxisColors[i] );
        glDrawArrays( GL_LINES, i << 1, 2 );
    }
}


void Scene::drawSelectionCentroid() const
{
    const GLfloat selectionCentroidColor[4] =
    {
        0.0f, 1.0f, 0.0f, 1.0f
    };

    // Bind selection centroid VAO and VBO as the active ones.
    glBindVertexArray( selectionCentroidVAO );
    glBindBuffer( GL_ARRAY_BUFFER, selectionCentroidVBO );

    // Set selection centroid color.
    glUniform4fv( uniformColorLocation, 1, selectionCentroidColor );

    // Draw selection centroid point.
    // TODO: The range of point sizes are implementation-dependent. Also I have to
    // check wheter point size mode is enabled or not.
    glPointSize( 3.0f );
    glDrawArrays( GL_POINTS, 0, 1 );
    glPointSize( 1.0f );
}

/*
void Scene::drawGuideRect() const
{
    const GLfloat guideRectColor[4] =
    {
        0.0f, 0.0f, 1.0f, 1.0f
    };

    glBindVertexArray( guideRectsVAO );
    glBindBuffer( GL_ARRAY_BUFFER, guideRectsVBO );

    // Set guide rect color.
    glUniform4fv( uniformColorLocation, 1, guideRectColor );

    glDisable( GL_DEPTH_TEST );
    glDrawArrays( GL_LINES, 6, 2 );
    glEnable( GL_DEPTH_TEST );
}
*/

} // namespace como

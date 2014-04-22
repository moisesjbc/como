/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "drawables_selection.hpp"

namespace como {


/***
 * 1. Initialization and destruction
 ***/

DrawablesSelection::DrawablesSelection() :
    centroid_( 0.0f, 0.0f, 0.0f, 1.0f ),
    pivotPointMode_( PivotPointMode::MEDIAN_POINT )
{
}


/***
 * 2. Getters
 ***/

glm::vec4 DrawablesSelection::getCentroid() const
{
    glm::vec4 centroid;

    mutex_.lock();
    centroid = centroid_;
    mutex_.unlock();

    return centroid;
}


PivotPointMode DrawablesSelection::getPivotPointMode() const
{
    PivotPointMode pivotPointMode;

    mutex_.lock();
    pivotPointMode = pivotPointMode_;
    mutex_.unlock();

    return pivotPointMode;
}


bool DrawablesSelection::contains( DrawableType drawableType ) const
{
    DrawablesMap::const_iterator it;

    mutex_.lock();

    for( it = drawables_.begin(); it != drawables_.end(); it++ ){
        if( it->second->getType() == drawableType ){
            mutex_.unlock();
            return true;
        }
    }

    mutex_.unlock();
    return false;
}


bool DrawablesSelection::contains( MeshType meshType ) const
{
    DrawablesMap::const_iterator it;
    const Drawable* drawable = nullptr;

    mutex_.lock();

    for( it = drawables_.begin(); it != drawables_.end(); it++ ){
        drawable = it->second.get();

        if( ( drawable->getType() == DrawableType::MESH ) &&
                ( dynamic_cast< const Mesh* >( drawable ) )->getType() == meshType ){
            mutex_.unlock();
            return true;
        }
    }

    mutex_.unlock();
    return false;
}


/***
 * 3. Setters
 ***/

void DrawablesSelection::setPivotPointMode( PivotPointMode pivotPointMode )
{
    mutex_.lock();
    pivotPointMode_ = pivotPointMode;
    emit selectionChanged();
    mutex_.unlock();
}

std::string DrawablesSelection::getName() const
{
    const Drawable* drawable = nullptr;

    if( getSize() == 0 ){
        return std::string( "Nothing selected" );
    }else if( getSize() == 1 ){
        drawable = drawables_.begin()->second.get();

        return std::string( drawable->getName() );
    }else{
        return getTypeName();
    }
}

std::string DrawablesSelection::getTypeName() const
{
    const Drawable* drawable = nullptr;

    if( getSize() == 0 ){
        return std::string( "Nothing selected" );
    }else if( getSize() == 1 ){
        drawable = drawables_.begin()->second.get();
        switch( drawable->getType() ){
            case DrawableType::MESH:
                switch( ( dynamic_cast< const Mesh* >( drawable ) )->getType() ){
                    case MeshType::MESH:
                        return std::string( "Mesh" );
                    break;
                    case MeshType::CAMERA:
                        return std::string( "Camera" );
                    break;
                    case MeshType::LIGHT:
                        switch( ( dynamic_cast< const Light* >( drawable ) )->getType() ){
                            case LightType::DIRECTIONAL_LIGHT:
                                return std::string( "Directional light" );
                            break;
                            default:
                                return std::string( "Undefined light" );
                            break;
                        }
                    break;
                    default:
                        return std::string( "Undefined mesh" );
                    break;
                }
            break;
            default:
                return std::string( "Undefined drawable" );
            break;
        }
    }else{
        return std::string( "Selection" );
    }
}


unsigned int DrawablesSelection::getSize() const
{
    unsigned int size = 0;

    mutex_.lock();
    size = drawables_.size();
    mutex_.unlock();

    return size;
}


/***
 * 4. Transformations
 ***/

void DrawablesSelection::translate( glm::vec3 direction )
{   
    DrawablesMap::iterator drawable;

    mutex_.lock();

    // Translate every drawable in the selection.
    for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
        drawable->second->translate( direction );
    }

    // Update the selection's centroid.
    updateSelectionCentroid();

    emit selectionChanged();

    mutex_.unlock();
}


void DrawablesSelection::rotate( GLfloat angle, glm::vec3 axis )
{
    DrawablesMap::iterator drawable;

    mutex_.lock();

    // Rotate every drawable in the selection according to the selected
    // pivot point mode.
    switch( pivotPointMode_ ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
            for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
                drawable->second->rotate( angle, axis, glm::vec3( drawable->second->getCentroid() ) );
            }
        break;
        case PivotPointMode::MEDIAN_POINT:
            for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
                drawable->second->rotate( angle, axis, glm::vec3( centroid_ ) );
            }
        break;
        case PivotPointMode::WORLD_ORIGIN:
            for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
                drawable->second->rotate( angle, axis );
            }
        break;
    }

    // Update the selection's centroid.
    updateSelectionCentroid();

    emit selectionChanged();

    mutex_.unlock();
}


void DrawablesSelection::scale( glm::vec3 scaleFactors )
{
    DrawablesMap::iterator drawable;

    mutex_.lock();

    // Scale every drawable in the selection according to the selected
    // pivot point mode.
    switch( pivotPointMode_ ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
            for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
                drawable->second->scale( scaleFactors, glm::vec3( drawable->second->getCentroid() ) );
            }
        break;
        case PivotPointMode::MEDIAN_POINT:
            for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
                drawable->second->scale( scaleFactors, glm::vec3( centroid_ ) );
            }
        break;
        case PivotPointMode::WORLD_ORIGIN:
            for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
                drawable->second->scale( scaleFactors );
            }
        break;
    }

    // Update the selection's centroid.
    updateSelectionCentroid();

    emit selectionChanged();

    mutex_.unlock();
}


/***
 * 5. Centroid updating
 ***/

void DrawablesSelection::updateSelectionCentroid()
{
    DrawablesMap::iterator drawable;

    // Initialize the selection's centroid.
    centroid_ = glm::vec4( 0.0f );

    // Sum every drawable's centroid in the selection.
    for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
        centroid_ += drawable->second->getCentroid();
    }

    // Get the selection centroid.
    centroid_ /= drawables_.size();
    centroid_.w = 1.0f;

    /*
    // Map the pivot point VBO to client memory and update the selection centroid
    // coordinates (for drawing).
    glBindBuffer( GL_ARRAY_BUFFER, selectionCentroidVBO );
    selectionCentroidBuffer = (GLfloat *)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );

    for( unsigned int i=0; i<3; i++ ){
        selectionCentroidBuffer[i] = selectionCentroid[i];
    }

    glUnmapBuffer( GL_ARRAY_BUFFER );
    */
}


/***
 * 6. Drawables management
 ***/


void DrawablesSelection::addDrawable( PackableDrawableID drawableID, DrawablePtr drawable )
{
    mutex_.lock();

    // Insert the given pair <ID, drawable> into the selection.
    drawables_[ drawableID ] = drawable;

    // Update the selection's centroid.
    updateSelectionCentroid();

    emit selectionChanged();

    mutex_.unlock();
}

// TODO: ¿Possible dead lock (if A moves a drawable to B and B moves a drawable
// to A at the same time)?
bool DrawablesSelection::moveDrawable( PackableDrawableID drawableID, DrawablesSelection& destinySelection )
{
    mutex_.lock();

    // Check if the required drawable is in this selection.
    if( drawables_.count( drawableID ) ){
        // Move drawable from current selection to destiny one.
        destinySelection.addDrawable( drawableID, drawables_.at( drawableID ) );

        // Erase drawable from current selection.
        drawables_.erase( drawableID );

        // Update the selection centroid.
        updateSelectionCentroid();

        // Drawable found and moved.
        emit selectionChanged();
        mutex_.unlock();
        return true;
    }else{

        // Drawable not found.
        mutex_.unlock();
        return false;
    }
}


// TODO: ¿Possible dead lock (if A moves all to B and B moves all to A at the
// same time)?
void DrawablesSelection::moveAll( DrawablesSelection& destinySelection )
{
    DrawablesMap::iterator drawable;

    mutex_.lock();

    // COPY every drawable from current selection to destiny.
    for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
        destinySelection.addDrawable( drawable->first, drawable->second );
    }

    // Clear the current selection.
    clear();

    emit selectionChanged();
    mutex_.unlock();
}


void DrawablesSelection::clear()
{
    mutex_.lock();

    // Clear the current selection.
    drawables_.clear();

    // Update the selection centroid.
    centroid_ = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );

    emit selectionChanged();
    mutex_.unlock();
}


/***
 * 7. Ray picking
 ***/

bool DrawablesSelection::intersect( glm::vec3 r0, glm::vec3 r1, PackableDrawableID& closestDrawable, float& minT ) const
{
    DrawablesMap::const_iterator drawable;
    float t;
    bool drawableIntersected = false;

    mutex_.lock();

    // Check if the given ray intersects any drawable in the selection.
    for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
        drawable->second->intersects( r0, r1, t );

        // New closest object, get its ID and distance.
        if( ( t >= 0.0f ) && ( t < minT ) ){
            drawableIntersected = true;

            closestDrawable = drawable->first;
            minT = t;
        }
    }

    return drawableIntersected;

    mutex_.unlock();
}


/***
 * 8. Drawing
 ***/

void DrawablesSelection::draw( const glm::mat4& viewProjMatrix, const GLfloat* contourColor ) const
{
    DrawablesMap::const_iterator drawable;

    mutex_.lock();

    // Draw every drawable in current selection.
    for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
        drawable->second->draw( viewProjMatrix, contourColor );
    }

    mutex_.unlock();
}

} // namespace como
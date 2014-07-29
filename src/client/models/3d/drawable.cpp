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

#include "drawable.hpp"
#include <stdexcept>

namespace como {


/***
 * 1. Initialization and destruction
 ***/

Drawable::Drawable( DrawableType type, std::string name ) :
    type_( type ),
    name_( name ),
    modelMatrix_( 1.0f )
{
    // Initialize the drawable's original orientation.
    originalOrientation[X] = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
    originalOrientation[Y] = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
    originalOrientation[Z] = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );
}


/***
 * 2. Getters
 ***/

glm::mat4 Drawable::getModelMatrix() const
{
    return modelMatrix_;
}


DrawableType Drawable::getType() const
{
    return type_;
}


std::string Drawable::getName() const
{
    return name_;
}


/***
 * 3. Transformations
 ***/

void Drawable::translate( glm::vec3 direction )
{
    // Compute the translation matrix.
    const glm::mat4 newTranslation = glm::translate( glm::mat4( 1.0f ), direction );

    applyTransformation( newTranslation );
}

#include <cstdlib>

void Drawable::rotate( GLfloat angle, glm::vec3 axis )
{
    // Normalize the vector "axis".
    axis = glm::normalize( axis );

    // Compute the rotation matrix for the given angle and the axis converted to
    // object space.
    const glm::mat4 newRotation = glm::rotate( glm::mat4( 1.0f ), angle, axis );

    applyTransformation( newRotation );
}


void Drawable::rotate( const GLfloat& angle, glm::vec3 axis, const glm::vec3& pivot )
{
    translate( -pivot );
    rotate( angle, axis );
    translate( pivot );
}


void Drawable::scale( glm::vec3 scaleFactors )
{
    // Compute the scale matrix.
    const glm::mat4 newScale = glm::scale( glm::mat4( 1.0f ), scaleFactors );

    applyTransformation( newScale );
}

void Drawable::scale( glm::vec3 scaleFactors, const glm::vec3& pivotPoint )
{
    translate( -pivotPoint );
    scale( scaleFactors );
    translate( pivotPoint );
}


void Drawable::applyTransformation( const glm::mat4& newTransformation )
{
    // Move the drawable from world to object space, then apply the new
    // transformation and finally move back the drawable from object to world
    // space.
    modelMatrix_ = newTransformation * modelMatrix_;

    // Update the transformed vertices using the original ones and the
    // previous transformation matrix.
    update();
}


/***
 * 5. Updating and drawing
 ***/

void Drawable::update()
{
    for( unsigned int i = 0; i<3; i++ ){
        transformedOrientation[i] = modelMatrix_ * originalOrientation[i];
    }
}

} // namespace como.

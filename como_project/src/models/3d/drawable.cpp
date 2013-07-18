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

#include "drawable.hpp"

/***
 * 1. Initialization
 ***/

namespace como {

Drawable::Drawable()
{
    // Initialize transformation matrix to identity matrix.
    transformationMatrix = glm::mat4( 1.0f );

    // Initialize the drawable's original orientation.
    originalOrientation[como::X] = glm::vec3( 1.0f, 0.0f, 0.0f );
    originalOrientation[como::Y] = glm::vec3( 0.0f, 1.0f, 0.0f );
    originalOrientation[como::Z] = glm::vec3( 0.0f, 0.0f, 1.0f );
}


/***
* 2. Setters and getters
 ***/

glm::mat4 Drawable::getTransformationMatrix()
{
    return transformationMatrix;
}


/***
 * 3. Transformations
 ***/

void Drawable::translate( const GLfloat& tx, const GLfloat& ty, const GLfloat& tz )
{
    // Multiply the drawable's transformation matrix by a translation one.
    transformationMatrix = glm::translate( transformationMatrix, glm::vec3( tx, ty, tz ) );

    // Update the transformed vertices using the original ones and the
    // previous transformation matrix.
    update();
}


void Drawable::rotate( const GLfloat& angle, const GLfloat& x, const GLfloat& y, const GLfloat& z )
{
    // Multiply the drawable's transformation matrix by a rotation one.
    transformationMatrix = glm::rotate( transformationMatrix, angle, glm::vec3( x, y, z ) );

    // Update the transformed vertices using the original ones and the
    // previous transformation matrix.
    update();
}

} // namespace como.
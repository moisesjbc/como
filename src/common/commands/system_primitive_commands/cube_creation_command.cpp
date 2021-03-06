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

#include "cube_creation_command.hpp"

namespace como {

CubeCreationCommand::CubeCreationCommand() :
    SystemPrimitiveCommand( SystemPrimitiveCommandType::CUBE_CREATION, NO_RESOURCE, NO_RESOURCE, NO_RESOURCE, 6 )
{
    addPackable( &cubeWidth_ );
    addPackable( &cubeHeight_ );
    addPackable( &cubeDepth_ );
}


CubeCreationCommand::CubeCreationCommand( const ResourceID& cubeID, const ResourceID& materialID, const ResourceID& firstTextureWallID, float width, float height, float depth, const glm::vec3& centroid ) :
    SystemPrimitiveCommand( SystemPrimitiveCommandType::CUBE_CREATION, cubeID, materialID, firstTextureWallID, 6, centroid ),
    cubeWidth_( width ),
    cubeHeight_( height ),
    cubeDepth_( depth )
{
    addPackable( &cubeWidth_ );
    addPackable( &cubeHeight_ );
    addPackable( &cubeDepth_ );
}


CubeCreationCommand::CubeCreationCommand( const CubeCreationCommand &b ) :
    SystemPrimitiveCommand( b ),
    cubeWidth_( b.cubeWidth_ ),
    cubeHeight_( b.cubeHeight_ ),
    cubeDepth_( b.cubeDepth_ )
{
    addPackable( &cubeWidth_ );
    addPackable( &cubeHeight_ );
    addPackable( &cubeDepth_ );
}


/***
 * 3. Getters
 ***/

float CubeCreationCommand::getCubeWidth() const
{
    return cubeWidth_.getValue();
}


float CubeCreationCommand::getCubeHeight() const
{
    return cubeHeight_.getValue();
}


float CubeCreationCommand::getCubeDepth() const
{
    return cubeDepth_.getValue();
}

} // namespace como

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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "cube_creation_command.hpp"

namespace como {


/***
 * 1. Initialization and destruction
 ***/

CubeCreationCommand::CubeCreationCommand() :
    DrawableCommand( DrawableCommandType::CUBE_CREATION, 0, NULL_DRAWABLE_ID ),
    color_({0})
{}

CubeCreationCommand::CubeCreationCommand( UserID userID, DrawableID drawableID, const std::uint8_t* color ) :
    DrawableCommand( DrawableCommandType::CUBE_CREATION, userID, drawableID ),
    color_({0})
{
    setColor( color );
}

CubeCreationCommand::CubeCreationCommand( const CubeCreationCommand& b ) :
    DrawableCommand( b )
{
    setColor( b.color_ );
}


/***
 * 2. Packing and unpacking
 ***/

char* CubeCreationCommand::pack( char* buffer ) const
{
    unsigned int i;

    // Pack DrawableCommand attributes.
    buffer = DrawableCommand::pack( buffer );

    // Pack CubeCreationCommand attributes.
    for( i = 0; i < 4; i++ ){
        packer::pack( color_[i], buffer );
    }

    // Return the updated buffer pointer.
    return buffer;
}


const char* CubeCreationCommand::CubeCreationCommand::unpack( const char* buffer )
{
    unsigned int i;

    // Unpack DrawableCommand attributes.
    buffer = DrawableCommand::unpack( buffer );

    // Unpack CubeCreationCommand attributes.
    for( i = 0; i < 4; i++ ){
        packer::unpack( color_[i], buffer );
    }

    // Return the updated buffer pointer.
    return buffer;
}


/***
 * 3. Getters
 ***/

std::uint16_t CubeCreationCommand::getPacketSize() const
{
    return ( DrawableCommand::getPacketSize() +
             sizeof( color_[0] ) * 4 );
}


const std::uint8_t* CubeCreationCommand::getColor() const
{
    return color_;
}


/***
 * 4. Setters
 ***/

void CubeCreationCommand::setColor( const std::uint8_t* color )
{
    unsigned int i = 0;
    for( ; i < 4; i++ ){
        color_[i] = color[i];
    }
}

} // namespace como

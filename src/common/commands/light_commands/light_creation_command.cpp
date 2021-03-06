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

#include "light_creation_command.hpp"

namespace como {


/***
 * 1. Construction
 ***/

LightCreationCommand::LightCreationCommand( LightType lightType, UserID userID, ResourceID lightID, const Color& lightColor ) :
    LightCommand( LightCommandType::LIGHT_CREATION, userID, lightID ),
    lightType_( lightType ),
    lightColor_( lightColor )
{
    addPackable( &lightType_ );
    addPackable( &lightColor_ );
}


LightCreationCommand::LightCreationCommand( const LightCreationCommand& b ) :
    LightCommand( b ),
    lightType_( b.lightType_ ),
    lightColor_( b.lightColor_ )
{
    addPackable( &lightType_ );
    addPackable( &lightColor_ );
}


/***
 * 3. Getters
 ***/

LightType LightCreationCommand::getLightType() const
{
    return lightType_.getValue();
}


Color LightCreationCommand::getLightColor() const
{
    return lightColor_.getValue();
}


/***
 * 4. Buffer pre-reading
 ***/

LightType LightCreationCommand::getLightType( const void* buffer )
{
    LightCommand lightCommand( LightCommandType::LIGHT_CREATION, NO_USER, NO_RESOURCE );

    return static_cast< LightType >( static_cast< const std::uint8_t* >( buffer )[lightCommand.getPacketSize()] );
}

} // namespace como

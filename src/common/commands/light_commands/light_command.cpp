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

#include "light_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

LightCommand::LightCommand() :
    TypeCommand( CommandTarget::LIGHT, LightCommandType::LIGHT_CREATION, NO_USER ),
    lightID_( NO_RESOURCE )
{
    addPackable( &lightID_ );
}


LightCommand::LightCommand( LightCommandType commandType, UserID userID, const ResourceID& lightID ) :
    TypeCommand( CommandTarget::LIGHT, commandType, userID ),
    lightID_( lightID )
{
    addPackable( &lightID_ );
}


LightCommand::LightCommand( const LightCommand &b ) :
    TypeCommand( b ),
    lightID_( b.lightID_ )
{
    addPackable( &lightID_ );
}


/***
 * 3. Getters
 ***/

ResourceID LightCommand::getResourceID() const
{
    return lightID_.getValue();
}

} // namespace como

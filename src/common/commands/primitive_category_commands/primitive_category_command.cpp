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

#include "primitive_category_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

PrimitiveCategoryCommand::PrimitiveCategoryCommand( UserID userID, ResourceID categoryID, PrimitiveCategoryCommandType commandType ) :
    TypeCommand( CommandTarget::PRIMITIVE_CATEGORY, commandType, userID ),
    categoryID_( categoryID )
{
    addPackable( &categoryID_ );
}


PrimitiveCategoryCommand::PrimitiveCategoryCommand( const PrimitiveCategoryCommand& b) :
    TypeCommand( b ),
    categoryID_( b.categoryID_ )
{
    addPackable( &categoryID_ );
}


/***
 * 3. Getters
 ***/

ResourceID PrimitiveCategoryCommand::getCategoryID() const
{
    return categoryID_.getValue();
}

} // namespace como

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

#ifndef IDS_HPP
#define IDS_HPP

#include <cstdint>

namespace como {

typedef std::uint16_t UserID;
typedef std::uint32_t DrawableIndex;


/*
 * A drawable is uniquely identified by a pair of values:
 * - The ID of the user who created it.
 * - A index to differ the drawable from others created by the same user.
 */

struct DrawableID {
    UserID creatorID;
    DrawableIndex drawableIndex;

    DrawableID() :
        creatorID( 0 ),
        drawableIndex( 0 )
    {}

    bool operator < ( const DrawableID& b ) const {
        return ( ( creatorID < b.creatorID ) ||
                 ( (creatorID == b.creatorID) && (drawableIndex < b.drawableIndex) ) );
    }

    bool operator == ( const DrawableID& b ) const {
        return ( ( creatorID == b.creatorID ) &&
                 (drawableIndex == b.drawableIndex ) );
    }
};

const DrawableID NULL_DRAWABLE_ID;

} // namespace como


#endif // IDS_HPP
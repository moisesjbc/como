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

#ifndef IDS_HPP
#define IDS_HPP

#include <common/packables/composite_packable.hpp>
#include <common/packables/packable_integer.hpp>
#include <common/packables/packable_integer.hpp>
#include <cstdint>
#include <common/ids/user_id.hpp>

namespace como {

typedef std::uint32_t DrawableIndex;
typedef PackableUint32< DrawableIndex > PackableDrawableIndex;

/*
 * A drawable is uniquely identified by a pair of values:
 * - The ID of the user who created it.
 * - A index to differ the drawable from others created by the same user.
 */

struct PackableDrawableID : public CompositePackable {
    PackableUserID creatorID;
    PackableDrawableIndex drawableIndex;


    /***
     * 1. Construction
     ***/
    PackableDrawableID();
    PackableDrawableID( const PackableDrawableID& b );
    PackableDrawableID( PackableDrawableID&& b );


    /***
     * 2. Destruction
     ***/
    ~PackableDrawableID() = default;


    /***
     * 3. Operators
     ***/
    bool operator < ( const PackableDrawableID& b ) const ;
    bool operator == ( const PackableDrawableID& b ) const ;

    PackableDrawableID& operator = (const PackableDrawableID& b );
    PackableDrawableID& operator = ( PackableDrawableID&& );    
};

/***
 * 4. Console output
 ***/
std::ostream& operator << ( std::ostream& os, const PackableDrawableID& id );

const PackableDrawableID NULL_DRAWABLE_ID;

} // namespace como


#endif // IDS_HPP

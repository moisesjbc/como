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

#ifndef TRIANGLES_RANGE_HPP
#define TRIANGLES_RANGE_HPP

namespace como {

// TODO: Delete
struct TrianglesRange
{
    unsigned int firstTriangleIndex;
    unsigned int nTriangles;

    TrianglesRange() :
        firstTriangleIndex( 0 ),
        nTriangles( 0 ){}
};

} // namespace como

#endif // TRIANGLES_RANGE_HPP
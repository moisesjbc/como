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

#ifndef PACKER_HPP
#define PACKER_HPP

#include <cstdint>

namespace como {

namespace packer {

/***
 * 1. Packing
 ***/
void pack( const std::uint8_t& value, char*& buffer );
void pack( const std::uint16_t& value, char*& buffer );
void pack( const std::uint32_t& value, char*& buffer );
void pack( const char* str, char*& buffer, const unsigned int n );


/***
 * 2. Unpacking
 ***/
void unpack( std::uint8_t& value, const char*& buffer );
void unpack( std::uint16_t& value, const char*& buffer );
void unpack( std::uint32_t& value, const char*& buffer );
void unpack( char* str, const char*& buffer, const unsigned int n );


/***
 * 2. Uint16 translation
 ***/
std::uint16_t translateToNetworkOrder( const std::uint16_t& value );
std::uint16_t translateFromNetworkOrder( const std::uint16_t& value );


/***
 * 3. Uint32 translation
 ***/
std::uint32_t translateToNetworkOrder( const std::uint32_t& value );
std::uint32_t translateFromNetworkOrder( const std::uint32_t& value );

} // namespace packer

} // namespace como

#endif // PACKER_HPP

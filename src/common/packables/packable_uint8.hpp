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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef PACKABLE_UINT8_HPP
#define PACKABLE_UINT8_HPP

#include "packable_wrapper.hpp"

template <class UnpackedType>
class PackableUint8 : public PackableWrapper<UnpackedType>
{
    public:
        /***
         * 1. Construction
         ***/
        PackableUint8() = default;
        PackableUint8( const UnpackedType& value ) : PackableWrapper( value ){}
        PackableUint8( const PackableUint8& ) = default;
        PackableUint8( PackableUint8&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~PackableUint8() = default;


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const { return sizeof( std::uint8_t ); }


        /***
         * 4. Packing and unpacking
         ***/
        virtual void* pack( void* buffer ) const ;
        virtual const void* unpack( const void* buffer ) ;


        /***
         * 6. Operators
         ***/
        PackableUint8<UnpackedType>& operator = ( const PackableUint8<UnpackedType>& b );
        PackableUint8<UnpackedType>& operator = ( const UnpackedType& UnpackedType );
        PackableUint8<UnpackedType>& operator = ( PackableUint8<UnpackedType>&& ) = delete;
};


/***
 * 5. Packing and unpacking
 ***/

template <class UnpackedType >
void* PackableUint8<UnpackedType>::pack( void* buffer ) const
{
    // Cast the buffer.
    std::uint8_t* castedBuffer = static_cast< std::uint8_t* >( buffer );

    // Pack the wrapper's inner value into the buffer.
    *castedBuffer = static_cast< std::uint8_t >( value_ );

    // Return a pointer to the next position in buffer.
    return static_cast< void* >( castedBuffer + 1 );
}


template <class UnpackedType>
const void* PackableUint8<UnpackedType>::unpack( const void* buffer )
{
    // Cast buffer to the UnpackedType type.
    const UnpackedType* castedBuffer = static_cast< const UnpackedType* >( buffer );

    // Unpack the wrapper's inner valued from the buffer.
    value_ = *castedBuffer;

    // Return a pointer to the next position in buffer.
    return static_cast< const void* >( castedBuffer + 1 );
}

#endif // PACKABLE_UINT8_HPP
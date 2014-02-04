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

#include "selection_response_command.hpp"
#include <stdexcept>

namespace como {

/***
 * 1. Initialization and destruction
 ***/

SelectionResponseCommand::SelectionResponseCommand() :
    SceneCommand( SceneCommandType::SELECTION_RESPONSE, 0 ), // UserID: 0 - Server
    nSelections_( 0 ),
    selectionConfirmed_( 0 )
{
}


SelectionResponseCommand::SelectionResponseCommand( const SelectionResponseCommand& b ) :
    SceneCommand( b ),
    nSelections_( b.nSelections_ ),
    selectionConfirmed_( b.selectionConfirmed_ )
{
}


/***
 * 2. Packing and unpacking
 ***/

char* SelectionResponseCommand::pack( char* buffer ) const
{
    // Pack the ScemeCommand fields.
    buffer = SceneCommand::pack( buffer );

    // Pack the number of selection responses.
    packer::pack( nSelections_, buffer );

    // Pack the selection responses.
    packer::pack( selectionConfirmed_, buffer );

    // Return the buffer updated pointer.
    return buffer;
}


const char* SelectionResponseCommand::unpack( const char* buffer )
{
    // Unpack the ScemeCommand fields.
    buffer = SceneCommand::unpack( buffer );

    // Unpack the number of selection responses.
    packer::unpack( nSelections_, buffer );

    // Unpack the selection responses.
    packer::unpack( selectionConfirmed_, buffer );

    // Return the buffer updated pointer.
    return buffer;
}


/***
 * 3. Getters
 ***/

std::uint16_t SelectionResponseCommand::getPacketSize() const
{
    return SceneCommand::getPacketSize() +
            sizeof( nSelections_ ) +
            sizeof( selectionConfirmed_ );
}


std::uint8_t SelectionResponseCommand::getNSelections() const
{
    return nSelections_;
}


std::uint32_t SelectionResponseCommand::getSelectionConfirmed() const
{
    return selectionConfirmed_;
}


/***
 * 4. Setters
 ***/

void SelectionResponseCommand::addSelectionConfirmation( bool confirmed )
{
    std::uint8_t confirmationFlag = confirmed ? 1 : 0;

    if( nSelections_ >= MAX_SELECTION_CONFIRMATIONS ){
        throw std::runtime_error( "Selection confirmations exceeds the limit" );
    }

    // Add the confirmation flag to the bits flag.
    confirmationFlag <<= nSelections_;
    selectionConfirmed_ |= confirmationFlag;

    nSelections_++;
}


void SelectionResponseCommand::clear()
{
    nSelections_ = 0;
    selectionConfirmed_ = 0;
}

} // namespace como
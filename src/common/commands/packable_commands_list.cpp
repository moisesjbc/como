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

#include "packable_commands_list.hpp"
#include <iostream>

namespace como {


PackableCommandsList::PackableCommandsList( const PackableCommandsList& b ) :
    Packable( b ),
    commands_( b.commands_ )
{}


/***
 * 3. Packing and unpacking
 ***/

void* PackableCommandsList::pack( void* buffer ) const
{
    CommandsList::const_iterator it;
    PackableUint8< std::uint8_t > nCommands;

    // Retrieve the number of commands and pack it.
    nCommands.setValue( commands_.size() );
    buffer = nCommands.pack( buffer );

    // Pack the commands.
    for( it = commands_.begin(); it != commands_.end(); it++ ){
        buffer = (*it)->pack( buffer );
    }

    return buffer;
}


const void* PackableCommandsList::unpack( const void* buffer )
{
    PackableUint8< std::uint8_t > nCommands;
    CommandPtr command;
    unsigned int i;

    // Unpack the number of commands.
    buffer = nCommands.unpack( buffer );

    // Preread each command's type and cast the command to the appropiate
    // derived type. Then unpack.
    for( i = 0; i < nCommands.getValue(); i++ ){
        switch( Command::getTarget( buffer ) ){
            // User commands
            case CommandTarget::USER:
                switch( UserCommand::getType( static_cast< const std::uint8_t* >( buffer ) + 3 ) ){ // TODO: buffer+1 is ugly.
                    case UserCommandType::USER_CONNECTION:
                        command = CommandPtr( new UserConnectionCommand );
                    break;
                    case UserCommandType::USER_DISCONNECTION:
                        command =  CommandPtr( new UserDisconnectionCommand );
                    break;
                    case UserCommandType::PARAMETER_CHANGE:
                        command = CommandPtr( new ParameterChangeCommand );
                    break;
                }
            break;

            // Drawable commands
            case CommandTarget::DRAWABLE:
                switch( DrawableCommand::getType( static_cast< const std::uint8_t* >( buffer ) + 3 ) ){ // TODO: buffer+1 is ugly.
                    case DrawableCommandType::CUBE_CREATION:
                        command =  CommandPtr( new CubeCreationCommand );
                    break;
                    case DrawableCommandType::DRAWABLE_SELECTION:
                        command = CommandPtr( new DrawableSelectionCommand );
                    break;
                }
            break;

            // Selection commands
            case CommandTarget::SELECTION:
                switch( SelectionCommand::getType( static_cast< const std::uint8_t* >( buffer ) + 3 ) ){ // TODO: buffer+1 is ugly.
                    case SelectionCommandType::SELECTION_RESPONSE:
                        command = CommandPtr( new SelectionResponseCommand );
                    break;
                    case SelectionCommandType::FULL_DESELECTION:
                        command =  CommandPtr( new FullDeselectionCommand );
                    break;
                    case SelectionCommandType::SELECTION_DELETION:
                        command = CommandPtr( new SelectionDeletionCommand );
                    break;
                    case SelectionCommandType::SELECTION_TRANSFORMATION:
                        command = CommandPtr( new SelectionTransformationCommand );
                    break;
                }
            break;
        }

        buffer = command->unpack( buffer );
        commands_.push_back( command );
    }

    return buffer;
}


const void* PackableCommandsList::unpack( const void* buffer ) const
{
    const PackableUint8< std::uint8_t > nCommands( commands_.size() );
    CommandsList::const_iterator it;

    // Unpack the number of commands.
    buffer = nCommands.unpack( buffer );

    // Unpack the commands.
    for( it = commands_.begin(); it != commands_.end(); it++ ){
        buffer = (*it)->unpack( buffer );
    }

    return buffer;
}


/***
 * 4. Getters
 ***/

std::uint16_t PackableCommandsList::getPacketSize() const
{
    CommandsList::const_iterator it;
    std::uint16_t packetSize = sizeof( std::uint8_t );

    for( it = commands_.begin(); it != commands_.end(); it++ ){
        packetSize += (*it)->getPacketSize();
    }

    return packetSize;
}


const CommandsList* PackableCommandsList::getCommands() const
{
    return &commands_;
}


/***
 * 5. Commands management
 ***/

void PackableCommandsList::addCommand( CommandConstPtr command )
{
    commands_.push_back( command );
}


void PackableCommandsList::clear()
{
    commands_.clear();
}


} // namespace como
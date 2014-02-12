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

#ifndef USER_CONNECTION_COMMAND_HPP
#define USER_CONNECTION_COMMAND_HPP

#include "user_command.hpp"
#include "../../packets/user_acceptance_packet.hpp"

namespace como {

class UserConnectionCommand : public UserCommand
{
    private:
        char name_[NAME_SIZE];
        std::uint8_t selectionColor_[4];

    public:
        /***
         * 1. Initialization and destruction
         ***/
        UserConnectionCommand( UserID userID = 0 );
        UserConnectionCommand( const UserAcceptancePacket& userAcceptancePacket );
        UserConnectionCommand( const UserConnectionCommand& b );
        UserConnectionCommand( UserConnectionCommand&& ) = delete;

        ~UserConnectionCommand() = default;


        /***
         * 2. Packing and unpacking
         ***/
        virtual char* pack( char* buffer ) const ;
        virtual const char* unpack( const char* buffer ) ;


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const ;
        const char* getName() const ;
        const std::uint8_t* getSelectionColor() const ;


        /***
         * 4. Setters
         ***/
        void setName( const char* name );
        void setSelectionColor( const std::uint8_t& r, const std::uint8_t& g, const std::uint8_t& b, const std::uint8_t& a );


        /***
         * 5. Operators
         ***/
        UserConnectionCommand& operator=( const UserConnectionCommand& ) = delete;
        UserConnectionCommand& operator=( UserConnectionCommand&& ) = delete;
};

typedef std::shared_ptr< const UserConnectionCommand > UserConnectionCommandConstPtr;

} // namespace como

#endif // USER_CONNECTION_COMMAND_HPP

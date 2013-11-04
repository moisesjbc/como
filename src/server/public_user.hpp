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

#ifndef PUBLIC_USER_HPP
#define PUBLIC_USER_HPP

#include <memory>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/bind.hpp>
#include <functional>
#include "../common/packets/packets.hpp"
#include <list>

namespace como {

const unsigned int MAX_COMMANDS_PER_PACKET = 4;

const unsigned int BUFFER_SIZE = 1024;

typedef boost::asio::ip::tcp::socket Socket;
typedef std::shared_ptr< Socket > SocketPtr;

class PublicUser : public std::enable_shared_from_this<PublicUser>
{
    private:
        unsigned int id_;
        SocketPtr socket_;

        char name_[64];

        std::function<void (unsigned int)> removeUserCallback_;

        std::uint8_t nextCommand_;
        std::uint8_t nCommandsInLastPacket_;

        SceneUpdate outSceneUpdatePacket_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        PublicUser( unsigned int id, const char* name, Socket socket, std::function<void (unsigned int)> removeUserCallback );
        ~PublicUser();


        /***
         * 2. Getters
         ***/
        unsigned int getId();
        const char* getName();


        /***
         * 3. Setters
         ***/


        /***
         * 4. Socket reading
         ***/


        /***
         * 5. Socket writing
         ***/
        bool needsSceneUpdate( const CommandsList* commandsHistoric ) const ;
        void sendNextSceneUpdate( const CommandsList* commandsHistoric );
};


typedef std::shared_ptr< PublicUser > PublicUserPtr;

} // namespace como

#endif // PUBLIC_USER_HPP

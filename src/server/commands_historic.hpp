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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef COMMANDS_HISTORIC_HPP
#define COMMANDS_HISTORIC_HPP
#include <mutex>
#include <thread>

#include <list>
#include <common/packets/packets.hpp>
#include <common/utilities/lockable.hpp>

namespace como {

typedef std::list< CommandConstPtr > CommandsList;

class CommandsHistoric : public Lockable
{
    private:
        // List of commands in the historic.
        CommandsList commands_;

        // Function to be called when a command is added to the historic.
        std::function< void () > broadcastCallback_;

    public:
        /***
         * 1. Construction
         ***/
        CommandsHistoric() = delete;
        CommandsHistoric( const CommandsHistoric& ) = delete;
        CommandsHistoric( CommandsHistoric&& ) = delete;
        CommandsHistoric( std::function< void () > broadcastCallback );


        /***
         * 2. Destruction
         ***/
        ~CommandsHistoric() = default;


        /***
         * 3. Getters
         ***/
        unsigned int getSize() const ;


        /***
         * 4. Historic modification
         ***/
        void addCommand( CommandConstPtr command );


        /***
         * 5. Auxiliar methods
         ***/
        std::uint32_t fillSceneUpdatePacketPacket( SceneUpdatePacket& packet,
                                             const unsigned int firstCommand,
                                             const unsigned int nCommands,
                                             UserID userID ) const ;


        /***
         * 6. Operators
         ***/
        CommandsHistoric& operator = (const CommandsHistoric& ) = delete;
        CommandsHistoric& operator = ( CommandsHistoric&& ) = delete;


    private:
        /***
         * 7. Getters (private)
         ***/
        bool mustSendCommandToUser( const Command& command, const UserID& userID ) const;
};

typedef std::shared_ptr< CommandsHistoric > CommandsHistoricPtr;
typedef std::shared_ptr< const CommandsHistoric > CommandsHistoricConstPtr;

}

#endif // COMMANDS_HISTORIC_HPP

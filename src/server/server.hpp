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

#ifndef SERVER_HPP
#define SERVER_HPP

#include <ctime>
#include <string>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include "public_user.hpp"
#include <common/packets/packets.hpp>
#include <common/utilities/log.hpp>
#include "commands_historic.hpp"
#include <map>
#include <queue>
#include <server/managers/server_primitives_manager.hpp>
#include <common/scene/basic_scene.hpp>
#include <server/managers/scene.hpp>
#include <common/ids/resource_ids_generator.hpp>
#include <common/utilities/lockable.hpp>

using boost::asio::ip::tcp;

namespace como {

typedef std::map< ResourceID, UserID > DrawableOwners;

/*! Main server manager */
class Server : public Lockable
{
    private:
        ResourceIDsGeneratorPtr resourceIDsGenerator_;

        LogPtr log_;

        // I/O service.
        std::shared_ptr< boost::asio::io_service > io_service_;

        // Acceptor for new TCP connections
        boost::asio::ip::tcp::acceptor acceptor_;

        // Work object.
        boost::asio::io_service::work work_;

        // Users map (ID, user).
        UsersMap users_;

        // Number of worker threads in the server.
        const unsigned int N_THREADS;

        // Maximum number of sessions (users) allowed in the server.
        const unsigned int MAX_SESSIONS;

        // Threads pool
        boost::thread_group threads_;

        // New socket
        Socket newSocket_;

        // Server's port.
        unsigned int port_;

        // Container of colors that aren't in use by any user.
        std::queue< std::uint32_t > freeUserColors_;

        // Historic of commands performed on the scene.
        CommandsHistoricPtr commandsHistoric_;

        Scene scene_;

    public:
        /***
         * 1. Construction
         ***/
        Server() = delete;
        Server( const Server& ) = delete;
        Server( Server&& ) = delete;

        /*! \brief Initialize a server.
         * \param port_ port the server will be listening to.
         * \param maxSessions maximum number of allowed simultaneous clients.
         * \param sceneName name of the scene to be created.
         * \param nThreads number of threads used during server execution.
         */
        Server( unsigned int port_, unsigned int maxSessions, const char* sceneName, const char* sceneFilePath, unsigned int nThreads = 3 );

    private:
        /*!
          * \brief Initialize the server's container of free colors for identifying users.
          */
        void initUserColors();

    public:
        /***
         * 2. Destruction
         ***/
        ~Server() = default;


        /***
         * 3. Main loop
         ***/
        /*! \brief Start the server's main loop. */
        void run();

        /*! \brief Disconnect from the server. */
        void disconnect();

    private:
        /*! \brief Notify to all the users that there is new commands to synchronize. */
        void broadcast();

        /***
         * 4. Listeners
         ***/
        /*! \brief Listen for a new connection */
        void listen();


        /***
         * 5. Handlers
         ***/
        /*! \brief Handler for a new connection */
        void onAccept( const boost::system::error_code& errorCode );

        /*! \brief Process a SCENE_UPDATE packet received from client
         * \param errorCode Error code associated with the packet reception.
         * \param userID ID of the user who sent the packet.
         * \param sceneUpdate SCENE_UPDATE packet received from client.
        */
        void processSceneUpdatePacket( const boost::system::error_code& errorCode,
                                 UserID userID,
                                 const SceneUpdatePacket& sceneUpdate );

        /*! \brief Process a scene command.
         * \param userID ID of the user who sent the command.
         * \param sceneCommand Scene command.
         */
        void processSceneCommand( const Command& sceneCommand );


        /***
         * 6. Commands historic management.
         ***/
        /*! \brief Add a command to the historic. */
        void addCommand( CommandConstPtr sceneCommand );


        /***
         * 7. Auxiliar methods
         ***/
    public:
        void deleteUser( UserID id );
    private:
        void workerThread();
        void openAcceptor();
        bool nameInUse( const char* newName ) const ;

    public:
        /***
         * 8. Operators
         ***/
        Server& operator = (const Server& ) = delete;
        Server& operator = ( Server&& ) = delete;
};

} // namespace como

#endif // SERVER_HPP

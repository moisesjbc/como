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

#include "server_interface.hpp"

namespace como {


/***
 * 1. Initialization and destruction
 ***/

ServerInterface::ServerInterface( LogPtr log ) :
    work_( std::shared_ptr< boost::asio::io_service::work >( new boost::asio::io_service::work( io_service_ ) ) ),
    socket_( SocketPtr( new boost::asio::ip::tcp::socket( io_service_ ) ) ),
    timer_( io_service_ ),
    log_( log )
{   
    for( unsigned int i=0; i<2; i++ ){
        workerThreads_.create_thread( boost::bind( &ServerInterface::work, this ) );
    }
}


ServerInterface::~ServerInterface()
{
    disconnect();
}


/***
 * 2. Connection and disconnection
 ***/

std::shared_ptr< const UserAccepted > ServerInterface::connect( const char* host, const char* port, const char* userName )
{
    boost::system::error_code errorCode;
    como::NewUser newUserPacket;
    como::UserAccepted userAcceptedPacket;
    const std::uint8_t* selectionColor;

    // Create the TCP resolver and query needed for connecting to the server.
    boost::asio::ip::tcp::resolver resolver( io_service_ );
    boost::asio::ip::tcp::resolver::query query( host, port );

    // Get the list of TCP endpoints given by resolving the previous query.
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve( query );

    // Connect to the server.
    log_->debug( "Connecting to the server...\n" );
    socket_->connect( *endpoint_iterator, errorCode );

    if( errorCode ){
        throw std::runtime_error( std::string( "ERROR: Couldn't connect to server (" ) + errorCode.message() + ")" );
    }

    log_->debug( "Connecting to the server...OK\n" );

    // Prepare a NEW_USER network package with the user name, and send it to
    // the server.
    newUserPacket.setName( userName );
    newUserPacket.send( *socket_ );

    // Read from the server an USER_ACCEPTED network package and unpack it.
    userAcceptedPacket.recv( *socket_ );

    selectionColor = userAcceptedPacket.getSelectionColor();
    log_->debug( "User accepted: \n",
                 "\tID: [", userAcceptedPacket.getId(), "]\n",
                 "\tName: [", userAcceptedPacket.getName(), "]\n",
                 "\tSelection color: [", (int)selectionColor[0], ", ",
                 (int)selectionColor[1], ", ",
                 (int)selectionColor[2], ", ",
                 (int)selectionColor[3], ")\n\n" );

    if( errorCode ){
        throw std::runtime_error( std::string( "ERROR when receiving USER_ACCEPTED package from server (" ) + errorCode.message() + ")" );
    }


    //listenerThread = new std::thread( std::bind( &ServerInterface::listen, this ) );
    listen();

    setTimer();

    return std::shared_ptr< const UserAccepted >( dynamic_cast< const UserAccepted* >( userAcceptedPacket.clone() ) );
}


void ServerInterface::disconnect()
{
    boost::system::error_code errorCode;

    log_->debug( "Disconnecting from server ...\n" );

    // Close the socket to the server if it's open.
    if( socket_->is_open() ){
        log_->debug( "Closing socket to server\n" );

        socket_->shutdown( boost::asio::ip::tcp::socket::shutdown_both, errorCode );
        socket_->close();
        log_->debug( "Closing socket to server... OK\n" );
    }

    // Stop the IO service and join the threads group.
    work_.reset();
    io_service_.stop();

    log_->debug( "Waiting for working threads to finish ...\n" );
    workerThreads_.join_all();
    log_->debug( "Waiting for working threads to finish ...OK\n" );

    log_->debug( "Disconnecting from server ...OK\n" );
}


/***
 * 3. Handlers
 ***/

void ServerInterface::onSceneUpdateReceived( const boost::system::error_code& errorCode, PacketPtr packet )
{
    if( errorCode ){
        log_->error( "Error when receiving packet: ", errorCode.message(), "\n" );
        return;
    }

    unsigned int i;
    const std::vector< SceneCommandConstPtr >* sceneCommands = nullptr;

    log_->lock();

    log_->debug( "Packet received\n" );

    SceneUpdate* sceneUpdate = dynamic_cast< SceneUpdate *>( packet.get() );
    //const UserConnected* userConnectedCommand = nullptr;

    if( !sceneUpdate ){
        throw std::runtime_error( std::string( "ERROR in \"onSceneUpdateReceived\": not a SCENE_UPDATE packet" ) );
    }

    log_->debug( "nCommands: ", sceneUpdate->getCommands()->size(), "\n" );

    log_->unlock();

    sceneCommands = sceneUpdate->getCommands();
    for( i=0; i<sceneCommands->size(); i++ ){
        log_->debug( "Executing remote command\n" );
        // TODO: Make thread-safe.
        emit commandReceived( ( *sceneCommands )[i] );

        switch( ( ( *sceneCommands )[i] )->getType() ){
            case SceneCommandType::USER_CONNECTED:
                log_->debug( "\tCommand[", i, "]: USER_CONNECTED\n" );
            break;
            case SceneCommandType::USER_DISCONNECTED:
                log_->debug( "\tCommand[", i, "]: USER_DISCONNECTED\n" );
            break;
            case SceneCommandType::CREATE_CUBE:
                log_->debug( "\tCommand[", i, "]: CREATE_CUBE" );
            break;
        }
    }

    listen();
}


void ServerInterface::onSceneUpdateSended( const boost::system::error_code& errorCode, PacketPtr packet )
{
    if( errorCode ){
        log_->error( "Error when sending packet: ", errorCode.message(), "\n" );
        return;
    }

    log_->debug( "SCENE_UPDATE sent to the server - nCommands ", packet, "\n" );

    //log_->debug( "SCENE_UPDATE sent to the server - nCommands: (",
    //             ( ( dynamic_cast< SceneUpdate* >( packet.get() ) )->getCommands() )->size(),
    //             ")\n" );

    setTimer();
}


/***
 * 4. Server communication
 ***/

void ServerInterface::sendCommand( SceneCommandConstPtr sceneCommand )
{
    // Queue the new scene command.
    sceneCommandsToServer_.push( sceneCommand );
}


void ServerInterface::sendPendingCommands()
{
    unsigned int nCommands = 0;

    // TODO: Change 4 by MAX_COMMANDS_PER_PACKET.
    // Move commands from the queue to the SCENE_UPDATE packet.
    while( ( nCommands < 4 ) && !sceneCommandsToServer_.empty() ){
        log_->debug( "Adding command to SCENE_UPDATE packet to server - 1\n" );
        // TODO: Delete the second argument is not necessary in a SCENE_UPDATE
        // packet sent from client to server.
        sceneUpdatePacketToServer_.addCommand( sceneCommandsToServer_.front(), 0, 0 );
        log_->debug( "Adding command to SCENE_UPDATE packet to server - 2\n" );
        sceneCommandsToServer_.pop();
        log_->debug( "Adding command to SCENE_UPDATE packet to server - 3\n" );

        nCommands++;
    }

    // If there in the packet, send it to the server. Otherwise, set a timer
    // to call this method again.
    if( nCommands ){
        log_->debug( "Sending SCENE_UPDATE packet to the server - 1\n" );
        sceneUpdatePacketToServer_.asyncSend( socket_, std::bind( &ServerInterface::onSceneUpdateSended, this, std::placeholders::_1, std::placeholders::_2 ) );
        log_->debug( "Sending SCENE_UPDATE packet to the server - 2\n" );
    }else{
        //log_->debug( "No commands to send to the server\n" );
        setTimer();
    }
}


void ServerInterface::setTimer()
{
    // Set a timer for sending pending commands to the server.
    timer_.expires_from_now( boost::posix_time::milliseconds( 100 ) );
    timer_.async_wait( boost::bind( &ServerInterface::sendPendingCommands, this ) );
}


void ServerInterface::listen()
{   
    log_->debug( "Listening for new scene updates from server ...\n" );

    sceneUpdatePacketFromServer_.asyncRecv( socket_, std::bind( &ServerInterface::onSceneUpdateReceived, this, std::placeholders::_1, std::placeholders::_2 ) );
}


void ServerInterface::work()
{
    boost::system::error_code errorCode;
    bool exit = false;

    log_->debug( "[", boost::this_thread::get_id(), "] thread start\n" );

    while( !exit ){
        try{
            log_->debug( "[", boost::this_thread::get_id(), "] io_service::run() ... \n" );
            io_service_.run( errorCode );
            log_->debug( "[", boost::this_thread::get_id(), "] io_service::run() ...OK\n" );
            if( errorCode ){
                log_->error( "[", boost::this_thread::get_id(), "] Error: ", errorCode.message(), "\n" );
            }
            exit = true;
        }catch( std::exception& ex ){
            // TODO: close the server correctly.
            throw ex;
        }
    }

    log_->debug( "[", boost::this_thread::get_id(), "] thread end\n" );
}

} // namespace como

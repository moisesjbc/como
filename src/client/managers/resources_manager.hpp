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

#ifndef RESOURCES_MANAGER_HPP
#define RESOURCES_MANAGER_HPP

#include <client/managers/server_interface/server_interface.hpp>
#include <common/utilities/observable_container/observable_container.hpp>
#include <common/commands/command.hpp>
#include <common/commands/resource_commands/resource_commands.hpp>
#include <common/managers/abstract_resources_ownership_manager.hpp>
#include <client/managers/selections/resources/resources_selection.hpp>
#include <client/managers/selections/resources/local_resources_selection.hpp>


namespace como {

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
class ResourcesManager : public AbstractResourcesOwnershipManager, public Observable, public Observer
{
    static_assert( std::is_base_of<ResourcesSelection<ResourceType>, ResourcesSelectionType>::value, "" );
    static_assert( std::is_base_of<LocalResourcesSelection<ResourceType>, LocalResourcesSelectionType>::value, "" );
    static_assert( std::is_base_of<ResourcesSelection<ResourceType>, LocalResourcesSelection<ResourceType>>::value, "" );


    public:
        /***
         * 1. Construction
         ***/
        ResourcesManager( ServerInterfacePtr server, LogPtr log );
        ResourcesManager() = delete;
        ResourcesManager( const ResourcesManager& ) = delete;
        ResourcesManager( ResourcesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ResourcesManager();


        /***
         * 3. Getters
         ***/
        std::shared_ptr< LocalResourcesSelectionType > getLocalResourcesSelection() const;
        virtual std::string getResourceName( const ResourceID& resourceID ) const = 0;


        /***
         * 4. Lock request
         ***/
        void requestResourceLock( const ResourceID& resourceID );
        void requestSelectionUnlock();
        void requestSelectionDeletion();


        /***
         * 5. Updating (Observer pattern)
         ***/
        virtual void update();


        /***
         * 6. Operators
         ***/
        ResourcesManager& operator = ( const ResourcesManager& ) = delete;
        ResourcesManager& operator = ( ResourcesManager&& ) = delete;


    protected:
        /***
         * 7. Selections management
         ***/
        void createResourcesSelection( UserID userID );
        void removeResourcesSelection( UserID userID );
        void addResourcesSelectionObserver( UserID userID, Observer* observer );
        void removeResourcesSelectionObserver( UserID userID, Observer* observer );


        /***
         * 8. Protected getters
         ***/
        std::shared_ptr< ResourcesSelectionType > getResourcesSelection( UserID userID ) const;


        /***
         * 9. Server communication
         ***/
        void sendCommandToServer( CommandConstPtr command );


        /***
         * 10. Server info
         ***/
        UserID localUserID() const;
        ResourceID newResourceID();
        ServerInterfacePtr server() const; // TODO: Make this method unnecessary and remove it.


        /***
         * 11. Resource management
         ***/
        virtual void lockResource( const ResourceID& resourceID, UserID userID ) = 0;
        virtual void unlockResourcesSelection( UserID userID ) = 0;
        virtual void deleteResourcesSelection( UserID userID ) = 0;
        virtual void processLockResponse( const ResourceID& resourceID, bool lockResponse );


    private:
        ServerInterfacePtr server_;

        std::queue< ResourceID > pendingSelections_;


    protected:
        std::map< UserID, std::shared_ptr< ResourcesSelectionType > > resourcesSelections_;

        std::shared_ptr< ResourcesSelectionType > nonSelectedResources_;
        std::shared_ptr< LocalResourcesSelectionType > localResourcesSelection_;
};


/***
 * 1. Construction
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::ResourcesManager( ServerInterfacePtr server, LogPtr log ) :
    AbstractResourcesOwnershipManager( log ),
    server_( server )
{
    createResourcesSelection( NO_USER );
    nonSelectedResources_ = resourcesSelections_.at( NO_USER );

    resourcesSelections_[localUserID()] = std::shared_ptr< ResourcesSelectionType >( new LocalResourcesSelectionType( server_ ) );
    localResourcesSelection_ = std::dynamic_pointer_cast< LocalResourcesSelectionType >( resourcesSelections_.at( localUserID() ) );
    localResourcesSelection_->Observable::addObserver( this );
}


/***
 * 2. Destruction
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::~ResourcesManager()
{
    for( auto& resourcesSelectionPair : resourcesSelections_ ){
        resourcesSelectionPair.second->Observable::removeObserver( this );
    }
}


/***
 * 3. Getters
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
std::shared_ptr<LocalResourcesSelectionType> ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::getLocalResourcesSelection() const
{
    return localResourcesSelection_;
}


/***
 * 4. Lock request
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::requestResourceLock( const ResourceID& resourceID )
{
    pendingSelections_.push( resourceID );
    sendCommandToServer( CommandConstPtr( new ResourceCommand( ResourceCommandType::RESOURCE_LOCK, localUserID(), resourceID ) ) );
}


template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::requestSelectionUnlock()
{
    CommandConstPtr selectionUnlockCommand =
            CommandConstPtr( new ResourcesSelectionCommand( ResourcesSelectionCommandType::SELECTION_UNLOCK, localUserID() ) );
    sendCommandToServer( selectionUnlockCommand );
}


template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::requestSelectionDeletion()
{
    CommandConstPtr selectionDeletionCommand =
            CommandConstPtr( new ResourcesSelectionCommand( ResourcesSelectionCommandType::SELECTION_DELETION, localUserID() ) );
    sendCommandToServer( selectionDeletionCommand );
}


/***
 * 5. Updating (Observer pattern)
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::update()
{
    // Simply forward the notification.
    notifyObservers();
}


/***
 * 7. Selections management
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::createResourcesSelection( UserID userID )
{
    resourcesSelections_[userID] = std::shared_ptr< ResourcesSelectionType >( new ResourcesSelectionType );
    addResourcesSelectionObserver( userID, this );
}


template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::removeResourcesSelection(UserID userID)
{
    resourcesSelections_.erase( userID );
}


template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::addResourcesSelectionObserver( UserID userID, Observer *observer )
{
    resourcesSelections_.at( userID )->Observable::addObserver( observer );
}


template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::removeResourcesSelectionObserver( UserID userID, Observer *observer )
{
    resourcesSelections_.at( userID )->Observable::removeObserver( observer );
}


/***
 * 8. Protected getters
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
std::shared_ptr<ResourcesSelectionType> ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::getResourcesSelection( UserID userID ) const
{
    return resourcesSelections_.at( userID );
}


/***
 * 9. Server communication
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::sendCommandToServer( CommandConstPtr command )
{
    server_->sendCommand( command );
}


/***
 * 10. Server info
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
UserID ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::localUserID() const
{
    return server_->getLocalUserID();
}


template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
ResourceID ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::newResourceID()
{
    return server_->getNewResourceID();
}


template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
ServerInterfacePtr ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::server() const
{
    return server_;
}


/***
 * 11. Resource management
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::processLockResponse( const ResourceID& resourceID, bool lockResponse )
{
    if( pendingSelections_.front() == resourceID ){
        if( lockResponse ){
            lockResource( resourceID, localUserID() );
        }

        pendingSelections_.pop();
    }
}

} // namespace como

#endif // RESOURCES_MANAGER_HPP

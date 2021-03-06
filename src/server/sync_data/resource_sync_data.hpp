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

#ifndef RESOURCE_SYNC_DATA_HPP
#define RESOURCE_SYNC_DATA_HPP

#include "sync_data.hpp"

namespace como {

class ResourceSyncData : public SyncData
{
    public:
        /***
         * 1. Construction
         ***/
        ResourceSyncData( const Command* creationCommand, const ResourceID& id );
        ResourceSyncData() = delete;
        ResourceSyncData( const ResourceSyncData& ) = default;
        ResourceSyncData( ResourceSyncData&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~ResourceSyncData() = default;


        /***
         * 3. Getters
         ***/
        virtual std::list< CommandConstPtr > generateUpdateCommands() const;
        ResourceID resourceID() const;
        UserID resourceOwner() const;
        std::list< ResourceID > childResourceIDs() const;


        /***
         * 4. Setters
         ***/
        void setResourceOwner( UserID newOwner );
        void addChildResource( const ResourceID& childID );


        /***
         * 5. Updating
         ***/
        virtual void processCommand( const Command& command );


        /***
         * 6. Operators
         ***/
        ResourceSyncData& operator = ( const ResourceSyncData& ) = default;
        ResourceSyncData& operator = ( ResourceSyncData&& ) = default;


    private:
        const ResourceID resourceID_;
        UserID resourceOwner_;
        std::string resourceName_;
        std::list< ResourceID > childResourceIDs_;
};

typedef std::unique_ptr< ResourceSyncData > ResourceSyncDataPtr;

} // namespace como

#endif // RESOURCE_SYNC_DATA_HPP

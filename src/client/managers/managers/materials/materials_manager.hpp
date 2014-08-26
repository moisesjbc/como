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

#ifndef MATERIALS_MANAGER_HPP
#define MATERIALS_MANAGER_HPP

#include <list>
#include <map>
#include <client/managers/managers/materials/material_handler.hpp>
#include <client/managers/server_interface/server_interface.hpp>
#include <common/ids/resource_id.hpp>
#include <string>
#include <QObject>
#include <memory>
#include <common/commands/material_commands/material_commands.hpp>
#include <functional>
#include <common/utilities/observable_container/observable_container.hpp>
#include <common/mesh_info/material_info.hpp>
#include <client/managers/resources_manager.hpp>
#include <client/managers/selections/materials/local_materials_selection.hpp>

namespace como {

typedef std::map< ResourceID, UserID > MaterialsOwnershipMap;

class MaterialsManager : public QObject, public ResourcesManager< Material, MaterialsSelection, LocalMaterialsSelection >
{
    Q_OBJECT

    private:
        std::map< ResourceID, MaterialPtr > materials_;
        MaterialsOwnershipMap materialsOwners_;

        MaterialHandlerPtr materialHandler_;

    public:
        /***
         * 1. Creation
         ***/
        MaterialsManager() = delete;
        MaterialsManager( ServerInterfacePtr server, LogPtr log );
        MaterialsManager( const MaterialsManager& ) = delete;
        MaterialsManager( MaterialsManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~MaterialsManager() = default;


        /***
         * 3. Material creation
         ***/
    public:
        ResourceID createMaterial( const MaterialInfo& materialInfo );
        void createMaterials( const std::vector<MaterialInfo>& materialsInfo, ResourceID& firstMaterialID );
        void createRemoteMaterials( const std::vector< MaterialInfo >& materialsInfo, const ResourceID& firstMaterialID );
    //private:
        void createMaterial( ResourceID id, const MaterialInfo &materialInfo );



        /***
         * 4. Material selection
         ***/
    public:
        void selectMaterial( const ResourceID& id );
    private:
        void selectMaterial( UserID userID, const ResourceID& id );


        /***
         * 5. Getters
         ***/
    public:
        MaterialHandlerPtr getCurrentMaterial() const;
        virtual string getResourceName( const ResourceID& resourceID ) const;
        MaterialConstPtr getMaterial( const ResourceID& id ) const;
        std::vector< MaterialConstPtr > getMaterials( const ResourceID& firstMaterialID, unsigned int nMaterials ) const;


        /***
         * 6. Remote command execution
         ***/
        void executeRemoteCommand( MaterialCommandConstPtr command );


        /***
         * 7. Updating
         ***/
        virtual void update();


        /***
         * 7. Operators
         ***/
        MaterialsManager& operator = ( const MaterialsManager& ) = delete;
        MaterialsManager& operator = ( MaterialsManager&& ) = delete;


        /***
         * 8. Signals
         ***/
    signals:
        void materialSelectionConfirmed( MaterialHandlerPtr material );
        void materialSelectionDenied( ResourceID material );


        /***
         * 9. Auxiliar methods
         ***/
    public:
        void highlightMaterial( ResourceID materialID );
        void removeHighlights();

        // ResourcesManager interface

    protected:
        /***
         * 10. Resources management
         ***/
        virtual void lockResource( const ResourceID& resourceID, UserID userID );
        virtual void unlockResourcesSelection( UserID userID );
        virtual void deleteResourcesSelection( UserID userID );       
};

typedef std::shared_ptr< MaterialsManager > MaterialsManagerPtr;

} // namespace como

#endif // MATERIALS_MANAGER_HPP

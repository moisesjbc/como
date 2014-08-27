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

#include "meshes_manager.hpp"

namespace como {

/***
 * 1. Construction
 ***/

MeshesManager::MeshesManager( ServerInterfacePtr server, LogPtr log, MaterialsManagerPtr materialsManager ) :
    ResourceCommandsExecuter( server ),
    SpecializedEntitiesManager( server, log ),
    materialsManager_( materialsManager ),
    newMeshesDisplayVertexNormals_( false )
{
    displayEdges( MeshEdgesDisplayFrequency::ONLY_WHEN_SELECTED );
}


/***
 * 4. Getters
 ***/

string MeshesManager::getResourceName( const ResourceID& resourceID) const
{
    for( const auto& meshesSelection : resourcesSelections_ ){
        if( meshesSelection.second->containsResource( resourceID ) ){
            return meshesSelection.second->getResourceName( resourceID );
        }
    }

    throw std::runtime_error( "MeshesManager::getResourceName() - resource not found" );
}


ElementsMeetingCondition MeshesManager::displaysVertexNormals() const
{
    const ElementsMeetingCondition firstSelectionValue =
            resourcesSelections_.begin()->second->displaysVertexNormals();

    if( firstSelectionValue == ElementsMeetingCondition::SOME ){
        return ElementsMeetingCondition::SOME;
    }

    for( const auto& meshesSelection : resourcesSelections_ ){
        if( meshesSelection.second->displaysVertexNormals() != firstSelectionValue ){
            return ElementsMeetingCondition::SOME;
        }
    }

    return firstSelectionValue;
}


unsigned int MeshesManager::getTotalMeshes() const
{
    unsigned int totalMeshes = 0;

    for( const auto& meshesSelection : resourcesSelections_ ){
        totalMeshes += meshesSelection.second->size();
    }

    return totalMeshes;
}


/***
 * 5. Setters
 ***/

void MeshesManager::displayVertexNormals( bool display )
{
    newMeshesDisplayVertexNormals_ = display;

    for( auto& meshesSelection : resourcesSelections_ ){
        meshesSelection.second->displayVertexNormals( display );
    }
}


void MeshesManager::displayEdges( MeshEdgesDisplayFrequency frequency )
{
    switch( frequency ){
        case MeshEdgesDisplayFrequency::ALWAYS:
            getResourcesSelection( NO_USER )->displayEdges( true );
        break;
        case MeshEdgesDisplayFrequency::ONLY_WHEN_SELECTED:
            getResourcesSelection( NO_USER )->displayEdges( false );
        break;
    }
}


/***
 * 6. Meshes management
 ***/

ResourceID MeshesManager::createMesh( MeshVertexData vertexData, MeshOpenGLData oglData, const std::vector< PolygonGroupData >& polygonsGroups, const std::vector< MaterialConstPtr >& materials )
{
    std::unique_ptr< Mesh > mesh( new Mesh( vertexData, oglData, polygonsGroups, materials, newMeshesDisplayVertexNormals_ ) );

    ResourceID meshID = getLocalResourcesSelection()->addResource( std::move( mesh ) );

    return meshID;
}


void MeshesManager::createMesh( ResourceID meshID, MeshVertexData vertexData, MeshOpenGLData oglData, const std::vector< PolygonGroupData >& polygonsGroups, const std::vector< MaterialConstPtr >& materials )
{
    std::unique_ptr< Mesh > mesh( new Mesh( vertexData, oglData, polygonsGroups, materials, newMeshesDisplayVertexNormals_ ) );

    getResourcesSelection( meshID.getCreatorID() )->addResource( meshID, std::move( mesh ) );
}



/***
 * 4. Resources selections management
 ***/

void MeshesManager::registerUser( UserID userID )
{
    resourcesSelections_[ userID ];
}

void MeshesManager::removeUser( UserID userID )
{
    unlockResourcesSelection( userID );
    resourcesSelections_.erase( userID );
}


/***
 * 5. Locking
 ***/

void MeshesManager::lockResource( const ResourceID &resourceID, UserID newOwner )
{
    ResourcesManager::lockResource( resourceID, newOwner );

    materialsManager_->lockMeshMaterials( resourceID, newOwner );
}


void MeshesManager::unlockResourcesSelection( UserID currentOwner )
{
    ResourcesManager::unlockResourcesSelection( currentOwner );

    materialsManager_->unlockUserMaterials( currentOwner );
}


void MeshesManager::clearResourcesSelection( UserID currentOwner )
{
    ResourcesManager::clearResourcesSelection( currentOwner );

    materialsManager_->removeUserMaterials( currentOwner );
}


} // namespace como
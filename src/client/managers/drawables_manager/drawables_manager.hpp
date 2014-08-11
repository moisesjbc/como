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

#ifndef DRAWABLES_MANAGER_HPP
#define DRAWABLES_MANAGER_HPP

#include <client/managers/drawables_selection/drawables_selection.hpp>
#include <client/managers/drawables_selection/local_drawables_selection.hpp>
#include <client/managers/server_interface/server_interface.hpp>
#include <map>
#include <client/managers/resources_manager.hpp>

namespace como {

typedef std::map< UserID, DrawablesSelectionPtr > DrawablesSelections;

enum class MeshEdgesDisplayFrequency {
    ALWAYS,
    ONLY_WHEN_SELECTED
};


class DrawablesManager : public ResourcesManager, public ContainerObserver<ResourceID>
{
    private:
        DrawablesSelections drawablesSelections_;

        DrawablesSelectionPtr nonSelectedDrawables_;
        LocalDrawablesSelectionPtr localDrawablesSelection_;

        MeshEdgesDisplayFrequency meshEdgesDisplayFrequency_;


    public:
        /***
         * 1. Construction
         ***/
        DrawablesManager( ServerInterfacePtr server, const PackableColor& localSelectionBorderColor, LogPtr log );
        DrawablesManager( const DrawablesManager& ) = delete;
        DrawablesManager( DrawablesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~DrawablesManager();


        /***
         * 3. Getters
         ***/
        virtual string getResourceName( const ResourceID& resourceID) const;
        LocalDrawablesSelectionPtr getLocalUserSelection() const ;
    private:
        DrawablesSelectionPtr getUserSelection( UserID userID ) const ;
        DrawablesSelectionPtr getUserSelection( UserID userID );
    public:
        glm::vec3 getPivotPoint() const ;
        glm::vec3 getPivotPoint( UserID userID ) const ;
        bool existsDrawable( const ResourceID& id ) const;


        /***
         * 4. Setters
         ***/
        void displayEdges( MeshEdgesDisplayFrequency frequency );


        /***
         * 6. Drawables administration
         ***/
    public:
        ResourceID addDrawable( DrawablePtr drawable );
        void addDrawable( UserID userID, DrawablePtr drawable, ResourceID drawableID );


        /***
         * 7. Selections management
         ***/
        void addDrawablesSelection( UserID userID, const PackableColor& selectionBorderColor );

        void deleteSelection();
        void deleteSelection( const unsigned int& userId );


        /***
         * 8. Drawables (de)seletion.
         ***/
        void selectDrawable( ResourceID drawableID );
        void selectDrawable( ResourceID drawableID, UserID userID );

        void unselectAll();
        void unselectAll( UserID userId );

        ResourceID selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection, glm::vec3& worldCollisionPoint );


        /***
         * 10. Drawing
         ***/
        void drawAll( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const ;


        /***
         * 11. Command execution
         ***/
        void executeRemoteSelectionCommand( SelectionCommandConstPtr command );
        void executeRemoteParameterChangeCommand( UserParameterChangeCommandConstPtr command );


        /***
         * 11. Operators
         ***/
        DrawablesManager& operator=( const DrawablesManager& ) = delete;
        DrawablesManager& operator=( DrawablesManager&& ) = delete;

        void highlightProperty( const void* property );


        /***
         * 12. Resource management
         ***/
    protected:
        virtual void lockResource( const ResourceID& resourceID, UserID userID );
        virtual void unlockResourcesSelection( UserID userID );
        virtual void deleteResourcesSelection( UserID userID );


        /***
         * 13. Updating (Observer pattern)
         ***/
        virtual void update( ContainerAction lastContainerAction, ResourceID lastElementModified );   
};

typedef shared_ptr< DrawablesManager > DrawablesManagerPtr;

} // namespace como

#endif // DRAWABLES_MANAGER_HPP

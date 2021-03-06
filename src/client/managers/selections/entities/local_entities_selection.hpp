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

#ifndef LOCAL_ENTITIES_MANAGER_HPP
#define LOCAL_ENTITIES_MANAGER_HPP

#include <client/managers/selections/entities/entities_selection.hpp>
#include <client/managers/selections/lights/local_lights_selection.hpp>
#include <client/managers/selections/meshes/local_meshes_selection.hpp>
#include <client/managers/selections/cameras/local_cameras_selection.hpp>
#include <client/managers/server_interface/server_interface.hpp>
#include <client/managers/utilities/server_writer.hpp>

namespace como {

// Available pivot point modes.
enum class PivotPointMode : std::uint8_t
{
    SELECTION_CENTROID = 0,
    INDIVIDUAL_CENTROIDS,
    WORLD_ORIGIN
};

class LocalEntitiesSelection : public EntitiesSelection, public ServerWriter
{
    public:
        /***
         * 1. Construction
         ***/
        LocalEntitiesSelection( ServerInterfacePtr server,
                                LocalLightsSelection* lightsSelection,
                                LocalMeshesSelection* meshesSelection,
                                LocalCamerasSelection* camerasSelection,
                                PivotPointMode pivotPointMode = PivotPointMode::SELECTION_CENTROID );
        LocalEntitiesSelection() = delete;
        LocalEntitiesSelection( const LocalEntitiesSelection& ) = delete;
        LocalEntitiesSelection( LocalEntitiesSelection&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~LocalEntitiesSelection() = default;


        /***
         * 3. Getters
         ***/
        glm::vec3 graphicPivotPoint() const;
        PivotPointMode pivotPointMode() const;


        /***
         * 4. Setters
         ***/
        void setPivotPointMode( PivotPointMode pivotPointMode );


        /***
         * 5. Transformations
         ***/
        virtual void translate( glm::vec3 direction );
        virtual void rotateAroundPivot( GLfloat angle, glm::vec3 axis, glm::vec3 pivot);
        virtual void rotateAroundIndividualCentroids( GLfloat angle, glm::vec3 axis);
        virtual void scaleAroundPivot( glm::vec3 scaleFactors, glm::vec3 pivot);
        virtual void scaleAroundIndividualCentroids( glm::vec3 scaleFactors );

        void rotate( GLfloat angle, glm::vec3 axis );
        void scale( glm::vec3 scaleFactors );


        /***
         * 6. Auxiliar methods
         ***/
        void roundTransformationMagnitude( glm::vec3& v );
        void roundTransformationMagnitude( float& angle, glm::vec3& v );


        /***
         * 7. Operators
         ***/
        LocalEntitiesSelection& operator = ( const LocalEntitiesSelection& ) = delete;
        LocalEntitiesSelection& operator = ( LocalEntitiesSelection&& ) = delete;


    private:
        PivotPointMode pivotPointMode_;
};

} // namespace como

#endif // LOCAL_ENTITIES_MANAGER_HPP

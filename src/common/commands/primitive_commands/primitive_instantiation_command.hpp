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

#ifndef PRIMITIVE_MESH_CREATION_COMMAND_HPP
#define PRIMITIVE_MESH_CREATION_COMMAND_HPP

#include "primitive_command.hpp"
#include <common/packables/ids/packable_resource_id.hpp>
#include <common/packables/array/packable_array_3.hpp>
#include <common/packables/packable_float.hpp>

namespace como {

class PrimitiveInstantiationCommand : public PrimitiveCommand
{
    private:
        // ID of the primitive we are creating this mesh from.
        PackableResourceID primitiveID_;

        PackableResourceID meshID_;
        PackableResourceID materialID_;

        PackableArray3< PackableFloat, float > centroid_;

    public:
        /***
         * 1. Construction
         ***/
        PrimitiveInstantiationCommand();
        PrimitiveInstantiationCommand( UserID userID, ResourceID primitiveID, ResourceID drawableID, const ResourceID& materialID, const glm::vec3& centroid );
        PrimitiveInstantiationCommand( const PrimitiveInstantiationCommand& b );
        PrimitiveInstantiationCommand( PrimitiveInstantiationCommand&& ) = delete;
        COMMAND_CLONE_METHOD( PrimitiveInstantiationCommand )


        /***
         * 2. Destruction
         ***/
        virtual ~PrimitiveInstantiationCommand() = default;


        /***
         * 3. Getters
         ***/
        ResourceID getPrimitiveID() const;
        ResourceID getMeshID() const;
        ResourceID getMaterialID() const;
        glm::vec3 centroid() const;


        /***
         * 4. Operators
         ***/
        PrimitiveInstantiationCommand& operator=( const PrimitiveInstantiationCommand& ) = delete;
        PrimitiveInstantiationCommand& operator=( PrimitiveInstantiationCommand&& ) = delete;
};

} // namespace como

#endif // PRIMITIVE_MESH_CREATION_COMMAND_HPP

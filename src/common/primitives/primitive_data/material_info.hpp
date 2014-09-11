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

#ifndef MATERIAL_INFO_HPP
#define MATERIAL_INFO_HPP

#include <string>
#include <glm/glm.hpp>
#include "texture_info.hpp"
#include <memory>
#include <vector>
#include <fstream>
#include <common/primitives/primitive_file.hpp> // TODO: Move PrimitiveFile::readVec3() method to another class.

namespace como {

struct MaterialInfo
{
    std::string name;

    glm::vec4 color;

    glm::vec3 ambientReflectivity;
    glm::vec3 diffuseReflectivity;
    glm::vec3 specularReflectivity;
    float specularExponent;

    std::unique_ptr< TextureInfo > textureInfo;


    /***
     * 1. Construction
     ***/
    MaterialInfo();
    MaterialInfo( const MaterialInfo& b );
    MaterialInfo( MaterialInfo&& ) = default;


    /***
     * 2. Destruction
     ***/
    ~MaterialInfo() = default;


    /***
     * 3. File reading
     ***/
    void readFromFile( std::ifstream& file );


    /***
     * 4. File writting
     ***/
    void writeToFile( std::ofstream& file ) const;


    /***
     * 5. Operators
     ***/
    MaterialInfo& operator = ( const MaterialInfo& ) = delete;
    MaterialInfo& operator = ( MaterialInfo&& ) = default;
};

typedef std::vector< MaterialInfo > MaterialsInfoVector;

} // namespace como

#endif // MATERIAL_INFO_HPP

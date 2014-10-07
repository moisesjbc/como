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

#include "specialized_system_primitives_factory.hpp"

namespace como {

/***
 * 1. Construction
 ***/

SpecializedSystemPrimitivesFactory::SpecializedSystemPrimitivesFactory( ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, TextureWallsManager* textureWallsManager ) :
    ServerWriter( server ),
    meshesManager_( meshesManager ),
    textureWallsManager_( textureWallsManager ),
    materialsManager_( materialsManager )
{}


/***
 * 4. SystemPrimitiveData generation
 ***/

SystemPrimitiveData SpecializedSystemPrimitivesFactory::generatePrimitiveData()
{
    SystemPrimitiveData primitiveData;

    generateVertexData( primitiveData.vertexData );
    generateNormalData( primitiveData.vertexData, primitiveData.normalData );
    generateUVData( primitiveData.uvData );
    generateTrianglesGroups( primitiveData.trianglesGroups );
    generateOGLData( primitiveData );

    return primitiveData;
}


void SpecializedSystemPrimitivesFactory::generateNormalData(const MeshVertexData &vertexData, MeshNormalData &normalData)
{
    normalData.initFromMeshVertexData( vertexData );
}


void SpecializedSystemPrimitivesFactory::generateOGLData( SystemPrimitiveData &primitiveData )
{
    primitiveData.generateOGLData();
}


/***
 * 5. Auxiliar methods
 ***/

void SpecializedSystemPrimitivesFactory::generateHorizontalVerticesCircle(std::vector<glm::vec3> &vertices, float radius, unsigned int nDivisions, float height)
{
    const float angleStep = 2.0f * glm::pi<float>() / nDivisions;
    float currentAngle = 0.0f;
    unsigned int i;

    // Create center vertex
    vertices.push_back( glm::vec3( 0.0f,
                                   height,
                                   0.0f ) );

    // Create radial vertices.
    for( i = 0; i < nDivisions; i++ ){
        vertices.push_back(
                    glm::vec3( radius * cos( currentAngle ),
                               height,
                               radius * sin( currentAngle ) ) );
        currentAngle += angleStep;
    }
}


void SpecializedSystemPrimitivesFactory::generateHorizontalUVCircle(std::vector<glm::vec2> &vertices, unsigned int nDivisions)
{
    const float angleStep = 2.0f * glm::pi<float>() / (float)( nDivisions );
    float currentAngle = 0.0f;
    unsigned int i;

    // Create center vertex
    vertices.push_back( glm::vec2( 0.5f, 0.5f ) );

    // Create radial vertices.
    for( i = 0; i < nDivisions; i++ ){
        vertices.push_back(
                    glm::vec2( 0.5f + 0.5f * cos( currentAngle ),
                               0.5f + 0.5f * sin( currentAngle ) ) );
        currentAngle += angleStep;
    }
}


void SpecializedSystemPrimitivesFactory::generateTrianglesCircle( std::vector< IndicesTriangle >& triangles, unsigned int nDivisions, unsigned int centerVertexIndex, unsigned int firstRadialVertexIndex, bool increaseIndices )
{
    unsigned int i;
    unsigned int currentVertexIndex;

    if( increaseIndices ){
        for( i = 0; i < nDivisions - 1; i++ ){
            currentVertexIndex = firstRadialVertexIndex + i;
            triangles.push_back({ centerVertexIndex,
                                  currentVertexIndex,
                                  currentVertexIndex + 1 });
        }
        currentVertexIndex = firstRadialVertexIndex + i;
        triangles.push_back({ centerVertexIndex,
                              currentVertexIndex,
                              firstRadialVertexIndex });
    }else{
        for( i = 0; i < nDivisions - 1; i++ ){
            currentVertexIndex = firstRadialVertexIndex + i;
            triangles.push_back({ centerVertexIndex,
                                  currentVertexIndex + 1,
                                  currentVertexIndex });
        }
        currentVertexIndex = firstRadialVertexIndex + i;
        triangles.push_back({ centerVertexIndex,
                              firstRadialVertexIndex,
                              currentVertexIndex });
    }
}

} // namespace como

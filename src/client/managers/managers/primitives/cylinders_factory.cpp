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

#include "cylinders_factory.hpp"

namespace como {

/***
 * 1. Construction
 ***/

CylindersFactory::CylindersFactory( ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, TextureWallsManager *textureWallsManager ) :
    SpecializedSystemPrimitivesFactory( server, meshesManager, materialsManager, textureWallsManager )
{}


/***
 * 3. Local cylinders creation
 ***/

ResourceID CylindersFactory::createCylinder( float height, float radius, std::uint16_t nRadialVertices )
{
    const ResourceID meshID = reserveResourceIDs( 1 );
    const ResourceID materialID = reserveResourceIDs( 1 );
    const ResourceID firstTextureWallID = reserveResourceIDs( 3 );

    createCylinder( meshID, materialID, firstTextureWallID, height, radius, nRadialVertices );

    sendCommandToServer(
                CommandConstPtr(
                    new CylinderCreationCommand(
                        meshID,
                        materialID,
                        firstTextureWallID,
                        height,
                        radius,
                        nRadialVertices ) ) );

    return meshID;
}


/***
 * 4. Remote command execution
 ***/

void CylindersFactory::executeRemoteCommand( const CylinderCreationCommand &command )
{
    createCylinder( command.getMeshID(),
                    command.getMaterialID(),
                    command.getFirstTextureWallID(),
                    command.cylinderHeight(),
                    command.cylinderRadius(),
                    command.cylinderNRadialVertices() );
}

/***
 * 6. Primitive data generation
 ***/

void CylindersFactory::generateVertexData( MeshVertexData &vertexData )
{
    const float angleStep = 2.0f * glm::pi<float>() / cylinderNRadialVertices_;
    float currentAngle = 0.0f;
    int i;
    unsigned int currentVertexIndex;
    unsigned int currentTopVertexIndex;
    unsigned int currentBottomVertexIndex;

    // Create top vertices
    vertexData.vertices.push_back(
                glm::vec3( 0.0f,
                           cylinderHeight_ / 2.0f,
                           0.0f ) );

    for( i = 0; i < cylinderNRadialVertices_; i++ ){
        vertexData.vertices.push_back(
                    glm::vec3( cylinderRadius_ * cos( currentAngle ),
                               cylinderHeight_ / 2.0f,
                               cylinderRadius_ * sin( currentAngle ) ) );
        currentAngle += angleStep;
    }

    // Create bottom vertices
    vertexData.vertices.push_back(
                glm::vec3( 0.0f,
                           -cylinderHeight_ / 2.0f,
                           0.0f ) );

    currentAngle = 0.0f;
    for( i = 0; i < cylinderNRadialVertices_; i++ ){
        vertexData.vertices.push_back(
                    glm::vec3( cylinderRadius_ * cos( currentAngle ),
                               -cylinderHeight_ / 2.0f,
                               cylinderRadius_ * sin( currentAngle ) ) );
        currentAngle += angleStep;
    }


    // Create top face triangles
    const unsigned int TOP_CENTER_INDEX = 0;
    const unsigned int TOP_FIRST_RADIAL_VERTEX_INDEX = TOP_CENTER_INDEX + 1;
    for( i = 0; i < cylinderNRadialVertices_ - 1; i++ ){
        currentVertexIndex = TOP_FIRST_RADIAL_VERTEX_INDEX + i;
        vertexData.vertexTriangles.push_back({
                                                 TOP_CENTER_INDEX,
                                                 currentVertexIndex + 1,
                                                 currentVertexIndex });
    }
    currentVertexIndex = TOP_FIRST_RADIAL_VERTEX_INDEX + i;
    vertexData.vertexTriangles.push_back({
                                             TOP_CENTER_INDEX,
                                             TOP_FIRST_RADIAL_VERTEX_INDEX,
                                             currentVertexIndex });

    // Create bottom face triangles
    const unsigned int BOTTOM_CENTER_INDEX = cylinderNRadialVertices_ + 1;
    const unsigned int BOTTOM_FIRST_RADIAL_VERTEX_INDEX = BOTTOM_CENTER_INDEX + 1;
    for( i = 0; i < cylinderNRadialVertices_ - 1; i++ ){
        currentVertexIndex = BOTTOM_FIRST_RADIAL_VERTEX_INDEX + i;
        vertexData.vertexTriangles.push_back({
                                                 BOTTOM_CENTER_INDEX,
                                                 currentVertexIndex,
                                                 currentVertexIndex + 1 });
    }
    currentVertexIndex = BOTTOM_FIRST_RADIAL_VERTEX_INDEX + i;
    vertexData.vertexTriangles.push_back({
                                             BOTTOM_CENTER_INDEX,
                                             currentVertexIndex,
                                             BOTTOM_FIRST_RADIAL_VERTEX_INDEX });

    // Create radial face
    for( i = 0; i < cylinderNRadialVertices_ - 1; i++ ){
        currentTopVertexIndex = TOP_FIRST_RADIAL_VERTEX_INDEX + i;
        currentBottomVertexIndex = BOTTOM_FIRST_RADIAL_VERTEX_INDEX + i;

        vertexData.vertexTriangles.push_back({
                                                 currentTopVertexIndex,
                                                 currentBottomVertexIndex + 1,
                                                 currentBottomVertexIndex });

        vertexData.vertexTriangles.push_back({
                                                 currentTopVertexIndex,
                                                 currentTopVertexIndex + 1,
                                                 currentBottomVertexIndex + 1 });
    }
    currentTopVertexIndex = TOP_FIRST_RADIAL_VERTEX_INDEX + i;
    currentBottomVertexIndex = BOTTOM_FIRST_RADIAL_VERTEX_INDEX + i;
    vertexData.vertexTriangles.push_back({
                                             currentTopVertexIndex,
                                             BOTTOM_FIRST_RADIAL_VERTEX_INDEX,
                                             currentBottomVertexIndex });

    vertexData.vertexTriangles.push_back({
                                             currentTopVertexIndex,
                                             TOP_FIRST_RADIAL_VERTEX_INDEX,
                                             BOTTOM_FIRST_RADIAL_VERTEX_INDEX });
}


void CylindersFactory::generateUVData(MeshTextureData &uvData)
{
    const float angleStep = 2.0f * glm::pi<float>() / cylinderNRadialVertices_;
    float currentAngle = 0.0f;
    int i;
    unsigned int currentVertexIndex;
    unsigned int currentTopVertexIndex;
    unsigned int currentBottomVertexIndex;

    // Create top and bottom UV vertices
    uvData.uvVertices.push_back( glm::vec2( 0.5f, 0.5f ) );

    for( i = 0; i < cylinderNRadialVertices_; i++ ){
        uvData.uvVertices.push_back(
                    glm::vec2( 0.5f + 0.5f * cos( currentAngle ),
                               0.5f + 0.5f * sin( currentAngle ) ) );
        currentAngle += angleStep;
    }

    // Create radial top UV vertices.
    for( i = 0; i < cylinderNRadialVertices_; i++ ){
        uvData.uvVertices.push_back(
                    glm::vec2(
                        static_cast<float>( i ) / static_cast<float>( cylinderNRadialVertices_ ),
                        0.0f ) );
    }

    // Create radial bottom UV vertices.
    for( i = 0; i < cylinderNRadialVertices_; i++ ){
        uvData.uvVertices.push_back(
                    glm::vec2(
                        static_cast<float>( i ) / static_cast<float>( cylinderNRadialVertices_ ),
                        1.0f ) );
    }

    // Create top UV triangles
    const unsigned int TOP_CENTER_INDEX = 0;
    const unsigned int TOP_FIRST_RADIAL_VERTEX_INDEX = TOP_CENTER_INDEX + 1;
    for( i = 0; i < cylinderNRadialVertices_ - 1; i++ ){
        currentVertexIndex = TOP_FIRST_RADIAL_VERTEX_INDEX + i;
        uvData.uvTriangles.push_back({
                                         TOP_CENTER_INDEX,
                                         currentVertexIndex + 1,
                                         currentVertexIndex });
    }
    currentVertexIndex = TOP_FIRST_RADIAL_VERTEX_INDEX + i;
    uvData.uvTriangles.push_back({
                                     TOP_CENTER_INDEX,
                                     TOP_FIRST_RADIAL_VERTEX_INDEX,
                                     currentVertexIndex });

    // Create bottom UV triangles
    const unsigned int BOTTOM_CENTER_INDEX = 0;
    const unsigned int BOTTOM_FIRST_RADIAL_VERTEX_INDEX = BOTTOM_CENTER_INDEX + 1;
    for( i = 0; i < cylinderNRadialVertices_ - 1; i++ ){
        currentVertexIndex = BOTTOM_FIRST_RADIAL_VERTEX_INDEX + i;
        uvData.uvTriangles.push_back({
                                         BOTTOM_CENTER_INDEX,
                                         currentVertexIndex,
                                         currentVertexIndex + 1 });
    }
    currentVertexIndex = BOTTOM_FIRST_RADIAL_VERTEX_INDEX + i;
    uvData.uvTriangles.push_back({
                                     BOTTOM_CENTER_INDEX,
                                     currentVertexIndex,
                                     BOTTOM_FIRST_RADIAL_VERTEX_INDEX });



    // Create radial triangles
    const unsigned int TOP_SIDE_FIRST_RADIAL_VERTEX_INDEX = cylinderNRadialVertices_ + 1;
    const unsigned int BOTTOM_SIDE_FIRST_RADIAL_VERTEX_INDEX = cylinderNRadialVertices_ * 2 + 1;
    for( i = 0; i < cylinderNRadialVertices_ - 1; i++ ){
        currentTopVertexIndex = TOP_SIDE_FIRST_RADIAL_VERTEX_INDEX + i;
        currentBottomVertexIndex = BOTTOM_SIDE_FIRST_RADIAL_VERTEX_INDEX + i;

        uvData.uvTriangles.push_back({
                                                 currentTopVertexIndex,
                                                 currentBottomVertexIndex + 1,
                                                 currentBottomVertexIndex });

        uvData.uvTriangles.push_back({
                                                 currentTopVertexIndex,
                                                 currentTopVertexIndex + 1,
                                                 currentBottomVertexIndex + 1 });
    }
    currentTopVertexIndex = TOP_SIDE_FIRST_RADIAL_VERTEX_INDEX + i;
    currentBottomVertexIndex = BOTTOM_SIDE_FIRST_RADIAL_VERTEX_INDEX + i;
    uvData.uvTriangles.push_back({ currentTopVertexIndex,
                                   currentBottomVertexIndex,
                                   BOTTOM_SIDE_FIRST_RADIAL_VERTEX_INDEX });

    /*
    uvData.uvTriangles.push_back({
                                             currentTopVertexIndex,
                                             TOP_SIDE_FIRST_RADIAL_VERTEX_INDEX,
                                             BOTTOM_SIDE_FIRST_RADIAL_VERTEX_INDEX });
                                             */
    //uvData.uvTriangles.push_back({ 0, 0, 0 });
    uvData.uvTriangles.push_back({ 0, 0, 0 });
}


void CylindersFactory::generateTrianglesGroups( std::vector<NamedTrianglesGroup> &trianglesGroups )
{
    // Top face
    trianglesGroups.push_back( NamedTrianglesGroup( "Top", 0, cylinderNRadialVertices_ ) );

    // Bottom face
    trianglesGroups.push_back( NamedTrianglesGroup( "Bottom", cylinderNRadialVertices_, cylinderNRadialVertices_ ) );

    // Radial face
    trianglesGroups.push_back( NamedTrianglesGroup( "Radial", cylinderNRadialVertices_ * 2, cylinderNRadialVertices_ * 2 ) );
}


/***
 * 7. Remote cylinders creation
 ***/

void CylindersFactory::createCylinder( const ResourceID &cylinderID, const ResourceID &materialID, const ResourceID &firstTextureWallID, float height, float radius, std::uint16_t nRadialVertices )
{
    std::unique_ptr< Mesh > cylinder = nullptr;

    cylinderHeight_ = height;
    cylinderRadius_ = radius;
    cylinderNRadialVertices_ = nRadialVertices;

    cylinder = std::unique_ptr< Mesh >( new SystemMesh( cylinderID, materialID, firstTextureWallID, generatePrimitiveData(), *materialsManager_ ) );

    meshesManager_->addMesh( std::move( cylinder ), cylinderID );
}


} // namespace como

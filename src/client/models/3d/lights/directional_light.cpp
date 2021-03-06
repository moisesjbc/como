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

#include "directional_light.hpp"

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

namespace como {

const glm::vec4 DEFAULT_LIGHT_VECTOR = glm::vec4( 0.0f, 1.0f, 0.0f, 0.0f );

/***
 * 1. Construction
 ***/

DirectionalLight::DirectionalLight( const ResourceID& id, const Color& lightColor, const glm::vec3& lightVector, OpenGL& openGL ) :
    Light( id, LightType::DIRECTIONAL_LIGHT, lightColor, "data/system/primitives/directional_light.prim", openGL ), // TODO: Load material from file.
    directionalLightIndex_( lockShaderDirectionalLight( openGL ) )
{
    (void)( lightVector ); // TODO: Remove this argument.

    GLint currentShaderProgram = 0;
    char uniformName[64];

    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    assert( currentShaderProgram != 0 );

    // Get the location of this light's isValid in the GLSL shader program.
    sprintf( uniformName, "directionalLights[%u].isValid", directionalLightIndex_ );
    isValidLocation_ = openGL.getShaderVariableLocation( uniformName );
    assert( isValidLocation_ != -1 );

    // Enable this light in shader.
    openGL.setUniformInteger( isValidLocation_, 1 );

    // Get the location of the DirectionalLight::lightIndex variable in shader.
    sprintf( uniformName, "directionalLights[%u].lightIndex", directionalLightIndex_ );
    lightIndexLocation_ = openGL.getShaderVariableLocation( uniformName );
    assert( lightIndexLocation_ != -1 );

    // Get the location of the DirectionalLight::lightVector variable in shader.
    sprintf( uniformName, "directionalLights[%u].lightVector", directionalLightIndex_ );
    lightVectorLocation_ = openGL.getShaderVariableLocation( uniformName );
    assert( lightVectorLocation_ != -1 );

    // TODO: Use halfVector.
    halfVectorLocation_ = -1;

    // Set the light index in shader.
    openGL.setUniformInteger( lightIndexLocation_, getLightIndex() );

    // Initialize light vector in shader.
    update();
}


/***
 * 2. Destruction
 ***/

DirectionalLight::~DirectionalLight()
{
    // Disable this light in shader.
    glUniform1i( isValidLocation_, 0 );
}


/***
 * 3. Getters
 ***/

glm::vec3 DirectionalLight::getLightVector() const
{
    return lightVector_;
}

glm::vec3 DirectionalLight::getHalfVector() const
{
    return halfVector_;
}


GLint DirectionalLight::getLightIndex() const
{
    return directionalLightIndex_;
}


std::string DirectionalLight::typeName() const
{
    return "DirectionalLight";
}


/***
 * 4. Updating
 ***/

void DirectionalLight::update()
{
    // Update mesh associated to this light.
    Light::update();

    // Compute transformed light vector.
    lightVector_ = glm::vec3( modelMatrix_ * DEFAULT_LIGHT_VECTOR );
    if( lightVector_.length() != 0.0f ){
        lightVector_ =  glm::normalize( lightVector_ );
    }

    // TODO: Update half vector.
}


/***
 * 5. Lights management
 ***/

unsigned int DirectionalLight::getMaxLights()
{
    // TODO: Retrieve value from shader.
    return 4;
}


/***
 * 6. Shader communication
 ***/

void DirectionalLight::sendToShader( OpenGL &openGL, const glm::mat4& viewMatrix ) const
{
    Light::sendToShader( openGL, viewMatrix );

    openGL.setUniformVec3( lightVectorLocation_,
                           glm::mat3( viewMatrix ) * lightVector_ );
}


/***
 * 7. Drawing
 ***/

void DirectionalLight::draw( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec4* contourColor ) const
{
    openGL->setShadingMode( ShadingMode::SOLID_PLAIN );

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // Send this "mesh" and the MVP matrix to the shader.
    Mesh::sendToShader( *openGL, viewMatrix, projectionMatrix );

    for( const auto& trianglesGroup : trianglesGroups_ ){
        // Send this mesh's material to shader.
        //materials_[polygonsGroup.materialIndex]->sendToShader();

        glDrawElements( GL_TRIANGLES,
                        trianglesGroup.nTriangles * 3,
                        GL_UNSIGNED_INT,
                        ( std::intptr_t* )( trianglesGroup.firstTriangleIndex * 3 * sizeof( GL_UNSIGNED_INT ) ) );
    }

    // Set the color for the mesh's contour.
    // TODO: Use only one condition (force contourColor to be passed as a
    // reference?)
    if( displaysEdges() && ( contourColor != nullptr ) ){
        openGL->setShadingMode( ShadingMode::SOLID_PLAIN );

        sendColorToShader( *contourColor );

        // Now we'll draw mesh's contour. Set polygon mode for rendering
        // lines.
        // TODO: I still have to use polygon offset.
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        // Draw Mesh's contour
        drawTriangles();

        // Return polygon mode to previos GL_FILL.
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}


/***
 * 9. Auxiliar methods
 ***/

// FIXME: Duplicated code (Light::lockShaderLight()).
GLuint DirectionalLight::lockShaderDirectionalLight( OpenGL& openGL )
{
    char uniformName[64] = {0};
    GLint varLocation = -1;
    unsigned int currentLightIndex;

    for( currentLightIndex = 0; currentLightIndex < 4; currentLightIndex++ ){ // Retrieve MAX_DIRECTIONAL_LIGHTS from shader.
        sprintf( uniformName, "directionalLights[%u].isValid", currentLightIndex );

        varLocation = openGL.getShaderVariableLocation( uniformName );
        assert( varLocation != -1 );

        if( !( openGL.getShaderInteger( ShaderProgramType::DEFAULT, uniformName ) ) ){
            openGL.setUniformInteger( varLocation, 1 );
            return currentLightIndex;
        }
    }

    OpenGL::checkStatus( "DirectionalLight::lockShaderDirectionalLight()" );

    // TODO: Or return -1 (change return type to GLint)?
    throw std::runtime_error( "DirectionalLight::lockShaderDirectionalLight() - No free light struct in shader" );
}

} // namespace como

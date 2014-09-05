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

#ifndef MESH_VERTEX_DATA_HPP
#define MESH_VERTEX_DATA_HPP

#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <GL/gl.h>

namespace como {

typedef std::vector< glm::vec3 > VerticesVector;

typedef std::array< GLuint, 3 > IndicesTriangle;
typedef std::vector< IndicesTriangle > VertexTrianglesVector;

struct MeshVertexData {
    VerticesVector vertices;
    VertexTrianglesVector vertexTriangles;
};

} // namespace como

#endif // MESH_VERTEX_DATA_HPP

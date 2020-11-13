//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_MESH_HPP
#define VD_GAME_ENGINE_MESH_HPP

#include <engine/api/gl/GL.hpp>

#include <string>
#include <vector>

#include "Vertex.hpp"
#include "Material.hpp"

namespace vd::model {
    struct Mesh {
        std::vector<Vertex>	vertices;
        std::vector<GLuint>	indices;
        std::vector<Material> materials;

        Mesh()
            : vertices()
            , indices()
            , materials()
        {
        }
    };
    typedef std::shared_ptr<Mesh>	MeshPtr;
    typedef std::vector<MeshPtr>	MeshPtrVec;
}

#endif //VD_GAME_ENGINE_MESH_HPP

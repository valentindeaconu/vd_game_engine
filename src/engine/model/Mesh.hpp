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
    class Mesh {
    public:
        typedef std::vector<Vertex>     VertexVec;
        typedef std::vector<GLuint>     IndexVec;
        typedef std::vector<Material>   MaterialVec;

        Mesh();
        ~Mesh();

        VertexVec&      Vertices();
        IndexVec&       Indices();
        MaterialVec&    Materials();
    private:
        VertexVec	    m_Vertices;
        IndexVec	    m_Indices;
        MaterialVec     m_Materials;
    };

    typedef std::shared_ptr<Mesh>	MeshPtr;
    typedef std::vector<MeshPtr>	MeshPtrVec;
}

#endif //VD_GAME_ENGINE_MESH_HPP

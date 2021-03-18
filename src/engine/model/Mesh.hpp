//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_MESH_HPP
#define VD_GAME_ENGINE_MESH_HPP

#include <string>
#include <vector>

#include "Vertex.hpp"
#include "Material.hpp"

namespace vd::model {
    class Mesh2D {
    public:
        typedef std::vector<Vertex2D>   VertexVec;
        typedef std::vector<uint32_t>   IndexVec;
        typedef std::vector<Material>   MaterialVec;

        Mesh2D();

        VertexVec&      Vertices();
        IndexVec&       Indices();
        MaterialVec&    Materials();
    private:
        VertexVec	    m_Vertices;
        IndexVec	    m_Indices;
        MaterialVec     m_Materials;
    };

    typedef std::shared_ptr<Mesh2D>	Mesh2DPtr;
    typedef std::vector<Mesh2DPtr>	Mesh2DPtrVec;

    class Mesh3D {
    public:
        typedef std::vector<Vertex3D>   VertexVec;
        typedef std::vector<GLuint>     IndexVec;
        typedef std::vector<Material>   MaterialVec;

        Mesh3D();

        VertexVec&      Vertices();
        IndexVec&       Indices();
        MaterialVec&    Materials();
    private:
        VertexVec	    m_Vertices;
        IndexVec	    m_Indices;
        MaterialVec     m_Materials;
    };

    typedef std::shared_ptr<Mesh3D>	Mesh3DPtr;
    typedef std::vector<Mesh3DPtr>	Mesh3DPtrVec;
}

#endif //VD_GAME_ENGINE_MESH_HPP

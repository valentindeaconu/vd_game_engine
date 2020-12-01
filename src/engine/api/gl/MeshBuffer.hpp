#ifndef __MESH_BUFFER_HPP_
#define __MESH_BUFFER_HPP_

#include <engine/api/gl/Texture.hpp>

#include "GL.hpp"
#include "Buffer.hpp"

namespace vd::gl {
    class MeshBuffer : public Buffer {
    public:
        MeshBuffer();
        ~MeshBuffer();

        void Allocate(const vd::model::MeshPtr& pMesh) override;
        void Render() override;
        void CleanUp() override;
    private:
        GLuint m_VaoId;
        GLuint m_VboId;
        GLuint m_EboId;

        vd::model::MeshPtr m_pMesh;
    };
    typedef std::shared_ptr<MeshBuffer>	MeshBufferPtr;
}

#endif // !__MESH_BUFFER_HPP_

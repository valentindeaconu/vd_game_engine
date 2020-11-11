#ifndef __MESH_BUFFER_HPP_
#define __MESH_BUFFER_HPP_

#include <engine/foundation/GL.hpp>

#include <engine/model/Mesh.hpp>
#include <engine/model/Texture.hpp>

#include "Buffer.hpp"

namespace vd::buffer {
    class MeshBuffer : public Buffer {
    public:
        MeshBuffer();
        ~MeshBuffer();

        void Allocate(const vd::model::MeshPtr& meshPtr) override;
        void Render() override;
        void CleanUp() override;
    private:
        GLuint m_VaoId;
        GLuint m_VboId;
        GLuint m_EboId;

        vd::model::MeshPtr m_MeshPtr;
    };
    typedef std::shared_ptr<MeshBuffer>	MeshBufferPtr;
}

#endif // !__MESH_BUFFER_HPP_

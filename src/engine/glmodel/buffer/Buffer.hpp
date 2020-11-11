#ifndef __BUFFER_HPP_
#define __BUFFER_HPP_

#include <engine/model/Mesh.hpp>

namespace vd::buffer {
    class Buffer {
    public:
        virtual void Allocate(const vd::model::MeshPtr& meshPtr) = 0;
        virtual void Render() = 0;
        virtual void CleanUp() = 0;
    };
    typedef std::shared_ptr<Buffer>	BufferPtr;
    typedef std::vector<BufferPtr>	BufferPtrVec;
}

#endif // !__BUFFER_HPP_

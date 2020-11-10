//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_PATCHBUFFER_HPP
#define VD_GAME_ENGINE_PATCHBUFFER_HPP

#include <memory>

#include "Buffer.hpp"

namespace vd::buffer {
    class PatchBuffer : public Buffer {
    public:
        PatchBuffer();
        ~PatchBuffer();

        void allocate(const vd::model::MeshPtr& meshPtr);
        void render() override;
        void cleanUp() override;
    private:
        GLuint vaoId;
        GLuint vbId;
        size_t size;

        vd::model::MeshPtr meshPtr;
    };
    typedef std::shared_ptr<PatchBuffer>	PatchBufferPtr;
}


#endif //VD_GAME_ENGINE_PATCHBUFFER_HPP

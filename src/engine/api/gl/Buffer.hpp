//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_GL_BUFFER_HPP
#define VD_GAME_ENGINE_GL_BUFFER_HPP

#include <engine/model/Mesh.hpp>

#include <unordered_map>

#include "Enums.hpp"
#include "Component.hpp"

namespace vd::gl {

    class Buffer : public Component {
    public:
        Buffer();

        void Bind();
        void Unbind();

        uint32_t AddBuffer(BufferType type, size_t size, const void* data, BufferUsageType usage);
        void UpdateBufferData(BufferType type, size_t count, const void* data);
        void RemoveBuffer(BufferType type);

        void AttributeArray(uint32_t index, int32_t size, DataType type, uint32_t stride, const void* ptr);

        void PatchParameter(const PatchParameterType& parameter, int value);
        void PatchParameter(const PatchParameterType& parameter, const float* values);

        void DrawArrays(PrimitiveType type, size_t count);
        void DrawElements(PrimitiveType type, size_t count, DataType dataType);

    private:
        void OnCreate() override;
        void OnCleanUp() override;

        bool        m_Bound;
        uint32_t    m_VaoId;

        std::vector<uint32_t>                       m_AttribArrays;
        std::unordered_map<BufferType, uint32_t>    m_Buffers;
    };
    typedef std::shared_ptr<Buffer>	BufferPtr;
    typedef std::vector<BufferPtr>	BufferPtrVec;
}

#endif //VD_GAME_ENGINE_GL_BUFFER_HPP

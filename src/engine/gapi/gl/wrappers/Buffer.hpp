//
// Created by Vali on 11/11/2020.
//

#ifndef VDGE_GL_BUFFER_WRAPPER_HPP
#define VDGE_GL_BUFFER_WRAPPER_HPP

#include <engine/gapi/Buffer.hpp>

#include <unordered_map>
#include <vector>

#include "Enums.hpp"
#include "Component.hpp"

namespace vd::gl::wrappers {

    class Buffer : public Component {
    public:
        Buffer();

        void Bind();
        void Unbind();

        uint32_t AddBuffer(BufferType type, size_t size, const void* data, BufferUsageType usage);
        void UpdateBufferData(BufferType type, size_t count, const void* data, size_t bufferIndex = 0);
        void RemoveBuffer(BufferType type, size_t bufferIndex = 0);

        void AttributeArray(uint32_t index, 
                            size_t bufferIndex, 
                            int32_t size, 
                            DataType dataType, 
                            uint32_t stride, 
                            const void* ptr);

        void InstanceAttributeArray(uint32_t index, 
                                    size_t bufferIndex, 
                                    int32_t size, 
                                    DataType dataType, 
                                    uint32_t stride, 
                                    uint32_t divisor, 
                                    const void* ptr);

        void PatchParameter(const PatchParameterType& parameter, int value);
        void PatchParameter(const PatchParameterType& parameter, const float* values);

        void DrawArrays(PrimitiveType type, size_t count);
        void DrawArraysInstanced(PrimitiveType type, size_t count, size_t instanceCount);
        void DrawElements(PrimitiveType type, size_t count, DataType dataType);
        void DrawElementsInstanced(PrimitiveType type, size_t count, DataType dataType, size_t instanceCount);

    private:
        void OnCreate() override;
        void OnCleanUp() override;

        bool        m_Bound;
        uint32_t    m_VaoId;

        std::vector<uint32_t>                                   m_AttribArrays;
        std::unordered_map<BufferType, std::vector<uint32_t>>   m_Buffers;
    };
    typedef std::shared_ptr<Buffer>	BufferPtr;
    typedef std::vector<BufferPtr>	BufferPtrVec;
}

#endif //VDGE_GL_BUFFER_WRAPPER_HPP

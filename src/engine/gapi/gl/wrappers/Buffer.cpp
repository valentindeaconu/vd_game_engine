//
// Created by Vali on 2/17/2021.
//

#include "Buffer.hpp"

namespace vd::gl::wrappers {
    inline std::string ToString(BufferType t) {
        switch (t) {
            case eArrayBuffer: return "ARRAY BUFFER";
            case eAtomicCounterBuffer: return "ATOMIC COUNTER BUFFER";
            case eCopyReadBuffer: return "COPY READ BUFFER";
            case eCopyWriteBuffer: return "COPY WRITE BUFFER";
            case eDispatchIndirectBuffer: return "DISPATCH INDIRECT BUFFER";
            case eDrawIndirectBuffer: return "DRAW INDIRECT BUFFER";
            case eElementArrayBuffer: return "ELEMENT ARRAY BUFFER";
            case ePixelPackBuffer: return "PIXEL PACK BUFFER";
            case ePixelUnpackBuffer: return "PIXEL UNPACK BUFFER";
            case eQueryBuffer: return "QUERY BUFFER";
            case eShaderStorageBuffer: return "SHADER STORAGE BUFFER";
            case eTextureBuffer: return "TEXTURE BUFFER";
            case eTransformFeedbackBuffer: return "TRANSFORM FEEDBACK BUFFER";
            case eUniformBuffer: return "UNIFORM BUFFER";
        }

        return "UNKNOWN";
    }

    Buffer::Buffer()
        : m_VaoId(0)
        , m_Bound(true)
    {
    }

    void Buffer::OnCreate() {
        glGenVertexArrays(1, &m_VaoId);
    }

    void Buffer::OnCleanUp() {
        Bind();

        std::vector<uint32_t> v;
        v.reserve(m_Buffers.size());
        for (auto& buffer : m_Buffers) {
            for (auto& id : buffer.second) {
                v.emplace_back(id);
            }
        }

        glDeleteBuffers(v.size(), &v[0]);

        glDeleteVertexArrays(1, &m_VaoId);

        Unbind();
    }

    void Buffer::Bind() {
        PassIfCreated();

        if (!m_Bound) {
            m_Bound = true;
            glBindVertexArray(m_VaoId);
        }
    }

    void Buffer::Unbind() {
        PassIfCreated();

        if (m_Bound) {
            m_Bound = false;
            glBindVertexArray(0);
        }
    }

    uint32_t Buffer::AddBuffer(BufferType type, size_t size, const void* data, BufferUsageType usage) {
        PassIfCreated();

        if (!m_Buffers.contains(type)) {
            m_Buffers[type] = std::vector<uint32_t>();
        }

        uint32_t id;
        glGenBuffers(1, &id);

        Bind();

        glBindBuffer(type, id);
        glBufferData(type, size, data, usage);
        
        Unbind();

        m_Buffers[type].emplace_back(id);

        return id;
    }

    void Buffer::UpdateBufferData(BufferType type, size_t count, const void* data, size_t bufferIndex) {
        PassIfCreated();

        if (!m_Buffers.contains(type)) {
            throw RuntimeError("No buffer of type " + ToString(type) + " exists");
        }

        uint32_t id = m_Buffers[type][bufferIndex];

        Bind();
        glBindBuffer(type, id);
        // glBufferData(type, count, data, GL_DYNAMIC_DRAW);
        glBufferSubData(type, 0, count, data);
        Unbind();
    }

    void Buffer::RemoveBuffer(BufferType type, size_t bufferIndex) {
        PassIfCreated();

        if (!m_Buffers.contains(type)) {
            throw RuntimeError("No buffer of type " + ToString(type) + " exists");
        }

        uint32_t id = m_Buffers[type][bufferIndex];
        m_Buffers.erase(type);

        Bind();
        glDeleteBuffers(1, &id);
        Unbind();
    }

    bool Buffer::HasBuffer(BufferType type) const {
        return m_Buffers.contains(type);
    }

    void Buffer::AttributeArray(uint32_t index,
                                size_t bufferIndex,
                                int32_t size,
                                DataType dataType,
                                uint32_t stride,
                                const void *ptr) {
        PassIfCreated();

        if (!m_Buffers.contains(eArrayBuffer)) {
            throw RuntimeError("No buffer of type " + ToString(eArrayBuffer) + " exists");
        }

        if (bufferIndex >= m_Buffers[eArrayBuffer].size()) {
            throw RuntimeError("Buffer of type " + ToString(eArrayBuffer) + " with index " + std::to_string(bufferIndex) + " does not exists");
        }

        Bind();

        glBindBuffer(eArrayBuffer, m_Buffers[eArrayBuffer][bufferIndex]);

        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, dataType, GL_FALSE, stride, (GLvoid*)ptr);

        m_AttribArrays.emplace_back(index);

        glBindBuffer(eArrayBuffer, 0);

        Unbind();
    }

    void Buffer::InstanceAttributeArray(uint32_t index,
                                        size_t bufferIndex,
                                        int32_t size,
                                        DataType dataType,
                                        uint32_t stride,
                                        uint32_t divisor,
                                        const void *ptr) {
        PassIfCreated();

        if (!m_Buffers.contains(eArrayBuffer)) {
            throw RuntimeError("No buffer of type " + ToString(eArrayBuffer) + " exists");
        }

        if (bufferIndex >= m_Buffers[eArrayBuffer].size()) {
            throw RuntimeError("Buffer of type " + ToString(eArrayBuffer) + " with index " + std::to_string(bufferIndex) + " does not exists");
        }

        Bind();

        glBindBuffer(eArrayBuffer, m_Buffers[eArrayBuffer][bufferIndex]);

        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, dataType, GL_FALSE, stride, (GLvoid*)ptr);
        glVertexAttribDivisor(index, divisor);

        m_AttribArrays.emplace_back(index);

        glBindBuffer(eArrayBuffer, 0);

        Unbind();
    }

    void Buffer::PatchParameter(const PatchParameterType& parameter, int value) {
        PassIfCreated();

        glPatchParameteri(parameter, value);
    }

    void Buffer::PatchParameter(const PatchParameterType& parameter, const float *values) {
        PassIfCreated();

        glPatchParameterfv(parameter, values);
    }

    void Buffer::DrawArrays(PrimitiveType type, size_t count) {
        PassIfCreated();

        Bind();

        glDrawArrays(type, 0, count);

        Unbind();
    }


    void Buffer::DrawArraysInstanced(PrimitiveType type, size_t count, size_t instanceCount) {
        PassIfCreated();

        Bind();

        glDrawArraysInstanced(type, 0, count, instanceCount);

        Unbind();
    }


    void Buffer::DrawElements(PrimitiveType type, size_t count, DataType dataType) {
        PassIfCreated();

        Bind();

        glDrawElements(type, count, dataType, nullptr);

        Unbind();
    }

    void Buffer::DrawElementsInstanced(PrimitiveType type, size_t count, DataType dataType, size_t instanceCount) {
        PassIfCreated();

        Bind();

        glDrawElementsInstanced(type, count, dataType, nullptr, instanceCount);

        Unbind();
    }

}
//
// Created by Vali on 2/17/2021.
//

#include "Buffer.hpp"

namespace vd::gl {
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
            v.emplace_back(buffer.second);
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

        if (m_Buffers.contains(type)) {
            throw RuntimeError("A buffer of type " + ToString(type) + " already exists");
        }

        uint32_t id;
        glGenBuffers(1, &id);

        Bind();

        glBindBuffer(type, id);
        glBufferData(type, size, data, usage);

        Unbind();

        m_Buffers[type] = id;

        return id;
    }

    void Buffer::UpdateBufferData(BufferType type, size_t count, const void* data) {
        PassIfCreated();

        if (!m_Buffers.contains(type)) {
            throw RuntimeError("No buffer of type " + ToString(type) + " exists");
        }

        uint32_t id = m_Buffers[type];

        Bind();
        glBindBuffer(type, id);
        glBufferSubData(type, 0, count, data);
        Unbind();
    }

    void Buffer::RemoveBuffer(BufferType type) {
        PassIfCreated();

        if (!m_Buffers.contains(type)) {
            throw RuntimeError("No buffer of type " + ToString(type) + " exists");
        }

        uint32_t id = m_Buffers[type];
        m_Buffers.erase(type);

        Bind();
        glDeleteBuffers(1, &id);
        Unbind();
    }

    void Buffer::AttributeArray(uint32_t index, int32_t size, DataType type, uint32_t stride, const void *ptr) {
        PassIfCreated();

        Bind();

        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, GL_FALSE, stride, (GLvoid*)ptr);

        m_AttribArrays.emplace_back(index);

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

    void Buffer::DrawElements(PrimitiveType type, size_t count, DataType dataType) {
        PassIfCreated();

        Bind();
        glDrawElements(type, count, dataType, nullptr);
        Unbind();
    }

}
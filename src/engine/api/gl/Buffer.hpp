//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_GL_BUFFER_HPP
#define VD_GAME_ENGINE_GL_BUFFER_HPP

#include <engine/model/Mesh.hpp>

#include <unordered_map>

#include "GLTypes.hpp"

namespace vd::gl {
    namespace buffer {
        enum BufferType {
            eArrayBuffer = GL_ARRAY_BUFFER,
            eAtomicCounterBuffer = GL_ATOMIC_COUNTER_BUFFER,
            eCopyReadBuffer = GL_COPY_READ_BUFFER,
            eCopyWriteBuffer = GL_COPY_WRITE_BUFFER,
            eDispatchIndirectBuffer = GL_DISPATCH_INDIRECT_BUFFER,
            eDrawIndirectBuffer = GL_DRAW_INDIRECT_BUFFER,
            eElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,
            ePixelPackBuffer = GL_PIXEL_PACK_BUFFER,
            ePixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER,
            eQueryBuffer = GL_QUERY_BUFFER,
            eShaderStorageBuffer = GL_SHADER_STORAGE_BUFFER,
            eTextureBuffer = GL_TEXTURE_BUFFER,
            eTransformFeedbackBuffer = GL_TRANSFORM_FEEDBACK_BUFFER,
            eUniformBuffer = GL_UNIFORM_BUFFER
        };

        enum BufferUsageType {
            eStaticDraw = GL_STATIC_DRAW,
            eDynamicDraw = GL_DYNAMIC_DRAW
        };

        std::string ToString(BufferType t);
    }

    enum PrimitiveType {
        ePoints = GL_POINTS,
        eLineStrip = GL_LINE_STRIP,
        eLineLoop = GL_LINE_LOOP,
        eLines = GL_LINES,
        eLineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
        eLinesAdjacency = GL_LINES_ADJACENCY,
        eTriangleStrip = GL_TRIANGLE_STRIP,
        eTriangleFan = GL_TRIANGLE_FAN,
        eTriangles = GL_TRIANGLES,
        eTriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
        eTrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
        ePatches = GL_PATCHES
    };

    class Buffer {
    public:
        Buffer();
        ~Buffer();

        void Bind();
        void Unbind();

        uint32_t AddBuffer(buffer::BufferType type, size_t size, const void* data, buffer::BufferUsageType usage);
        void UpdateBufferData(buffer::BufferType type, size_t count, const void* data);
        void RemoveBuffer(buffer::BufferType type);

        void AttributeArray(uint32_t index, int32_t size, DataType type, uint32_t stride, const void* ptr);

        void DrawArrays(PrimitiveType type, size_t count);
        void DrawElements(PrimitiveType type, size_t count, DataType dataType);

    private:
        bool        m_Bound;
        uint32_t    m_VaoId;

        std::vector<uint32_t>                               m_AttribArrays;
        std::unordered_map<buffer::BufferType, uint32_t>    m_Buffers;
    };
    typedef std::shared_ptr<Buffer>	BufferPtr;
    typedef std::vector<BufferPtr>	BufferPtrVec;
}

#endif //VD_GAME_ENGINE_GL_BUFFER_HPP

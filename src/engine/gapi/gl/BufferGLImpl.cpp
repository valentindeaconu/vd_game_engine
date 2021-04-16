//
// Created by Vali on 2/17/2021.
//

#include "Buffer.hpp"

namespace vd::gl {

    Buffer::Buffer()
        : m_DynamicMemory(false)
        , m_MultipleInstances(false)
        , m_DataFragmentation(gapi::DataFragmentation::eAsPoints)
        , m_Count(0)
        , m_InstanceCount(0)
    {
    }

    void Buffer::Use() {
        m_Buffer->Bind();
    }

    void Buffer::Discard() {
        m_Buffer->Unbind();
    }

    void Buffer::AllocateStatic(gapi::DataFragmentation dataFragmentation,
                                const std::vector<gapi::AttributeType>& perVertexAttributes,
                                const std::vector<gapi::AttributeType>& perInstanceAttributes,
                                const std::vector<float>& perVertexData,
                                const std::vector<float>& perInstanceData,
                                const std::vector<uint32_t>& indices) {
        // If buffer was already allocated, just release it's memory and reallocate it
        if (m_Buffer) {
            m_Buffer->CleanUp();
            m_Buffer = nullptr;
        }
        m_Buffer = std::make_unique<wrappers::Buffer>();

        m_DataFragmentation = dataFragmentation;

        // This allocation function only works for float data - because a float has 4 bytes - so bit-shifting to the
        // left with 2 positions is equal to multiplying with 4
        // In order to make this code works for any data type, left bit-shifting with 2 can be replaced with multiplying
        // with the sizeof(<datatype>) value
        // Also, remember to transfer the datatype enum to the Buffer object

        uint64_t vertexDataSize = ComputeDataSize(perVertexAttributes, perVertexAttributes.size());
        uint64_t vertexDataSizeInBytes = (vertexDataSize << 2);

        uint64_t vertexCount = perVertexData.size() / vertexDataSize;
        uint64_t verticesDataSizeInBytes = vertexDataSizeInBytes * vertexCount;

        m_Count = vertexCount;

        m_Buffer->Create();
        m_Buffer->Bind();
        m_Buffer->AddBuffer(gl::eArrayBuffer,
                            verticesDataSizeInBytes,
                            &perVertexData[0],
                            gl::eStaticDraw);

        if (dataFragmentation == gapi::DataFragmentation::eAsPatches) {
            m_Buffer->PatchParameter(vd::gl::ePatchVertices, vertexCount);
        }

        if (dataFragmentation == gapi::DataFragmentation::eAsTriangles) {
            m_Buffer->AddBuffer(gl::eElementArrayBuffer,
                                (indices.size() << 2),
                                &indices[0],
                                gl::eStaticDraw);

            m_Count = indices.size();
        }

        uint64_t attrIndex = 0;
        uint64_t attrOffset = 0;
        for (auto& attr : perVertexAttributes) {
            uint8_t attrSize = ToAttributeSize(attr);

            m_Buffer->AttributeArray(attrIndex,
                                     0,
                                     attrSize,
                                     vd::gl::eFloat,
                                     vertexDataSizeInBytes,
                                     (GLvoid*)attrOffset);

            attrIndex += 1;
            attrOffset += (attrSize << 2);
        }

        if (!perInstanceAttributes.empty()) {
            m_MultipleInstances = true;

            uint64_t instanceDataSize = ComputeDataSize(perInstanceAttributes, perInstanceAttributes.size());
            uint64_t instanceDataSizeInBytes = (instanceDataSize << 2);

            uint64_t instanceCount = perInstanceData.size() / instanceDataSize;
            uint64_t instancesDataSizeInBytes = instanceDataSizeInBytes * instanceCount;

            m_InstanceCount = instanceCount;

            m_Buffer->AddBuffer(vd::gl::eArrayBuffer, instancesDataSizeInBytes, &perInstanceData[0],
                                vd::gl::eStaticDraw);

            attrOffset = 0;
            for (auto& attr : perVertexAttributes) {
                uint8_t attrSize = ToAttributeSize(attr);

                m_Buffer->InstanceAttributeArray(attrIndex,
                                                 0,
                                                 attrSize,
                                                 vd::gl::eFloat,
                                                 instanceDataSizeInBytes,
                                                 1,
                                                 (GLvoid *) attrOffset);

                attrIndex += 1;
                attrOffset += (attrSize << 2);
            }
        }

        m_Buffer->Unbind();
    }

    void Buffer::AllocateDynamic(gapi::DataFragmentation dataFragmentation,
                                 const std::vector<gapi::AttributeType>& perVertexAttributes,
                                 const std::vector<gapi::AttributeType>& perInstanceAttributes,
                                 uint64_t perVertexMaximumDataSize,
                                 uint64_t maximumIndices,
                                 uint64_t perInstanceMaximumDataSize) {
        m_DynamicMemory = true;

        // If buffer was already allocated, just release it's memory and reallocate it
        if (m_Buffer) {
            m_Buffer->CleanUp();
            m_Buffer = nullptr;
        }
        m_Buffer = std::make_unique<wrappers::Buffer>();

        m_DataFragmentation = dataFragmentation;

        // This allocation function only works for float data - because a float has 4 bytes - so bit-shifting to the
        // left with 2 positions is equal to multiplying with 4
        // In order to make this code works for any data type, left bit-shifting with 2 can be replaced with multiplying
        // with the sizeof(<datatype>) value
        // Also, remember to transfer the datatype enum to the Buffer object

        uint64_t vertexDataSize = ComputeDataSize(perVertexAttributes, perVertexAttributes.size());
        uint64_t vertexDataSizeInBytes = (vertexDataSize << 2);

        m_Buffer->Create();
        m_Buffer->Bind();
        m_Buffer->AddBuffer(gl::eArrayBuffer,
                            (perVertexMaximumDataSize << 2),
                            nullptr,
                            gl::eDynamicDraw);

        if (dataFragmentation == gapi::DataFragmentation::eAsTriangles) {
            m_Buffer->AddBuffer(gl::eElementArrayBuffer,
                                (maximumIndices << 2),
                                nullptr,
                                gl::eDynamicDraw);
        }

        uint64_t attrIndex = 0;
        uint64_t attrOffset = 0;
        for (auto& attr : perVertexAttributes) {
            uint8_t attrSize = ToAttributeSize(attr);

            m_Buffer->AttributeArray(attrIndex,
                                     0,
                                     attrSize,
                                     vd::gl::eFloat,
                                     vertexDataSizeInBytes,
                                     (GLvoid*)attrOffset);

            attrIndex += 1;
            attrOffset += (attrSize << 2);
        }

        if (!perInstanceAttributes.empty()) {
            m_MultipleInstances = true;

            uint64_t instanceDataSize = ComputeDataSize(perInstanceAttributes, perInstanceAttributes.size());
            uint64_t instanceDataSizeInBytes = (instanceDataSize << 2);

            m_Buffer->AddBuffer(vd::gl::eArrayBuffer, (perInstanceMaximumDataSize << 2), nullptr, vd::gl::eDynamicDraw);

            attrOffset = 0;
            for (auto& attr : perVertexAttributes) {
                uint8_t attrSize = ToAttributeSize(attr);

                m_Buffer->InstanceAttributeArray(attrIndex,
                                                 0,
                                                 attrSize,
                                                 vd::gl::eFloat,
                                                 instanceDataSizeInBytes,
                                                 1,
                                                 (GLvoid *) attrOffset);

                attrIndex += 1;
                attrOffset += (attrSize << 2);
            }
        }

        m_Buffer->Unbind();
    }

    void Buffer::UpdateVertexData(const std::vector<float>& perVertexData) {
        if (m_DynamicMemory) {
            m_Buffer->UpdateBufferData(vd::gl::eArrayBuffer,
                                       (perVertexData.size() << 2),
                                       &perVertexData[0],
                                       0);
        }
    }

    void Buffer::UpdateIndices(const std::vector<uint32_t>& indices) {
        if (m_DynamicMemory) {
            m_Buffer->UpdateBufferData(vd::gl::eElementArrayBuffer,
                                       (indices.size() << 2),
                                       &indices[0],
                                       0);
        }
    }

    void Buffer::UpdateInstanceData(const std::vector<float>& perInstanceData) {
        if (m_DynamicMemory) {
            m_Buffer->UpdateBufferData(vd::gl::eArrayBuffer,
                                       (perInstanceData.size() << 2),
                                       &perInstanceData[0],
                                       1);
        }
    }

    void Buffer::Draw() {
        if (m_DataFragmentation == gapi::DataFragmentation::eAsTriangles) {
            if (m_MultipleInstances) {
                m_Buffer->DrawElementsInstanced(vd::gl::eTriangles, m_Count, vd::gl::eFloat, m_InstanceCount);
            } else {
                m_Buffer->DrawElements(vd::gl::eTriangles, m_Count, vd::gl::eFloat);
            }
        } else {
            gl::PrimitiveType primitiveType = ToPrimitiveType(m_DataFragmentation);

            if (m_MultipleInstances) {
                m_Buffer->DrawArraysInstanced(primitiveType, m_Count,  m_InstanceCount);
            } else {
                m_Buffer->DrawArrays(primitiveType, m_Count);
            }
        }
    }

    void Buffer::Release() {
        m_Buffer->CleanUp();
    }

    uint8_t Buffer::ToAttributeSize(const gapi::AttributeType& attribute) {
        switch (attribute) {
            case gapi::AttributeType::eFloat1: return 1;
            case gapi::AttributeType::eFloat2: return 2;
            case gapi::AttributeType::eFloat3: return 3;
            case gapi::AttributeType::eFloat4: return 4;
        }
    }

    uint64_t Buffer::ComputeDataSize(const std::vector<gapi::AttributeType>& attributes, size_t index) {
        uint64_t totalSize = 0;

        size_t N = std::min(index, attributes.size());
        for (int i = 0; i < N; ++i) {
            totalSize += (uint64_t)attributes[i];
        }

        return totalSize;
    }

    gl::PrimitiveType Buffer::ToPrimitiveType(const gapi::DataFragmentation& dataFragmentation) {
        switch (dataFragmentation) {
            case gapi::DataFragmentation::eAsPoints:        return gl::PrimitiveType::ePoints;
            case gapi::DataFragmentation::eAsLines:         return gl::PrimitiveType::eLines;
            case gapi::DataFragmentation::eAsTriangles:     return gl::PrimitiveType::eTriangles;
            case gapi::DataFragmentation::eAsTriangleFans:  return gl::PrimitiveType::eTriangleFan;
            case gapi::DataFragmentation::eAsTriangleStrip: return gl::PrimitiveType::eTriangleStrip;
            case gapi::DataFragmentation::eAsPatches:       return gl::PrimitiveType::ePatches;
        }
        return eLines;
    }

}
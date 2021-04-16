//
// Created by Vali on 11/11/2020.
//

#ifndef VDGE_GL_BUFFER_HPP
#define VDGE_GL_BUFFER_HPP

#include <engine/gapi/Buffer.hpp>
#include <engine/gapi/gl/wrappers/Buffer.hpp>

#include <unordered_map>
#include <vector>

#include "Enums.hpp"
#include "Component.hpp"

namespace vd::gl {

    class Buffer : public gapi::Buffer {
    public:
        Buffer();

        void Use() override;
        void Discard() override;

        void AllocateStatic(gapi::DataFragmentation dataFragmentation,
                            const std::vector<gapi::AttributeType>& perVertexAttributes,
                            const std::vector<gapi::AttributeType>& perInstanceAttributes,
                            const std::vector<float>& perVertexData,
                            const std::vector<float>& perInstanceData,
                            const std::vector<uint32_t>& indices) override;

        void AllocateDynamic(gapi::DataFragmentation dataFragmentation,
                             const std::vector<gapi::AttributeType>& perVertexAttributes,
                             const std::vector<gapi::AttributeType>& perInstanceAttributes,
                             uint64_t perVertexMaximumDataSize,
                             uint64_t maximumIndices,
                             uint64_t perInstanceMaximumDataSize) override;

        void UpdateVertexData(const std::vector<float>& perVertexData) override;
        void UpdateIndices(const std::vector<uint32_t>& indices) override;
        void UpdateInstanceData(const std::vector<float>& perInstanceData) override;

        void Draw() override;

        void Release() override;
    private:
        static uint64_t             ComputeDataSize(const std::vector<gapi::AttributeType>&, size_t);
        static uint8_t              ToAttributeSize(const gapi::AttributeType& attribute);
        static gl::PrimitiveType    ToPrimitiveType(const gapi::DataFragmentation& dataFragmentation);

        std::unique_ptr<wrappers::Buffer> m_Buffer;

        bool                    m_DynamicMemory;
        bool                    m_MultipleInstances;
        gapi::DataFragmentation m_DataFragmentation;
        uint64_t                m_Count;
        uint64_t                m_InstanceCount;
    };
    typedef std::shared_ptr<Buffer> BufferPtr;
}

#endif //VDGE_GL_BUFFER_HPP

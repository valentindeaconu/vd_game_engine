//
// Created by vali on 4/16/21.
//

#ifndef VDGE_BUFFER_HPP
#define VDGE_BUFFER_HPP

#include <vector>
#include <cstdint>
#include <memory>

namespace vd::gapi {
    enum class DataFragmentation {
        eAsPoints = 0,
        eAsLines,
        eAsTriangles,
        eAsTriangleFans,
        eAsTriangleStrip,
        eAsPatches
    };

    typedef std::vector<uint8_t>    AttributeTypeVec;
    class AttributeType {
    public:
        static const uint8_t FLOAT_1 = 1;
        static const uint8_t FLOAT_2 = 2;
        static const uint8_t FLOAT_3 = 3;
        static const uint8_t FLOAT_4 = 4;

        static uint64_t ComputeDataSize(const AttributeTypeVec&, size_t);
    };

    class Buffer {
    public:
        virtual void Use() = 0;
        virtual void Discard() = 0;

        virtual void AllocateStatic(DataFragmentation dataFragmentation,
                                    const AttributeTypeVec& perVertexAttributes,
                                    const AttributeTypeVec& perInstanceAttributes,
                                    const std::vector<float>& perVertexData,
                                    const std::vector<float>& perInstanceData,
                                    const std::vector<uint32_t>& indices = std::vector<uint32_t>()) = 0;

        virtual void AllocateDynamic(DataFragmentation dataFragmentation,
                                     const AttributeTypeVec& perVertexAttributes,
                                     const AttributeTypeVec& perInstanceAttributes,
                                     uint64_t perVertexMaximumDataSize,
                                     uint64_t maximumIndices,
                                     uint64_t perInstanceMaximumDataSize) = 0;

        virtual void UpdateVertexData(const std::vector<float>& perVertexData, uint64_t vertexCount) = 0;
        virtual void UpdateIndices(const std::vector<uint32_t>& indices) = 0;
        virtual void UpdateInstanceData(const std::vector<float>& perInstanceData) = 0;

        virtual void Draw() = 0;

        virtual void Release() = 0;
    };
    typedef std::shared_ptr<Buffer> BufferPtr;
}


#endif //VDGE_BUFFER_HPP

//
// Created by vali on 4/16/21.
//

#include "Buffer.hpp"

namespace vd::gapi {

    uint64_t AttributeType::ComputeDataSize(const AttributeTypeVec& attributes, size_t index) {
        uint64_t totalSize = 0;

        size_t N = std::min(index, attributes.size());
        for (int i = 0; i < N; ++i) {
            totalSize += attributes[i];
        }

        return totalSize;
    }

}

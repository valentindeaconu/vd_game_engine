//
// Created by Vali on 12/1/2020.
//

#include "Vertex.hpp"

#include <engine/defines/Macros.hpp>

namespace vd::model {

    Vertex::Vertex(const gapi::AttributeTypeVec& attributes) {
        m_Attributes.reserve(attributes.size());
        m_Attributes.insert(m_Attributes.end(), attributes.begin(), attributes.end());

        // Compute data size for all attributes and resize the data vector
        // By resizing, data is initialized with zeros
        uint64_t dataSize = gapi::AttributeType::ComputeDataSize(m_Attributes, m_Attributes.size());
        m_Data.resize(dataSize);
    }

    Vertex::Vertex(const std::vector<float>& rawData) {
        m_Data.clear();
        m_Data.reserve(rawData.size());
        m_Data.insert(m_Data.begin(), rawData.begin(), rawData.end());
    }

    bool Vertex::SameAttributes(const gapi::AttributeTypeVec& attributes) const {
        return m_Attributes == attributes;
    }

    void Vertex::Assign(const gapi::AttributeTypeVec& attributes) {
        CORE_ASSERT(gapi::AttributeType::ComputeDataSize(attributes, attributes.size()) == m_Data.size(),
                    "Attributes does not match current data");

        m_Attributes.clear();
        m_Attributes.reserve(attributes.size());
        m_Attributes.insert(m_Attributes.begin(), attributes.begin(), attributes.end());
    }

    void Vertex::Assign(const std::vector<float>& rawData) {
        CORE_ASSERT(rawData.size() == m_Data.size(), "Raw data dimension is different than allocated data");

        m_Data.clear();
        m_Data.reserve(rawData.size());
        m_Data.insert(m_Data.begin(), rawData.begin(), rawData.end());
    }

    template <>
    float& Vertex::Attribute(size_t index) {
        CORE_ASSERT(index < m_Attributes.size(), "Requested attribute is not allocated");
        CORE_ASSERT(m_Attributes[index] == gapi::AttributeType::FLOAT_1, "Vertex attribute is not of type Float1!");

        uint64_t dataSizeBefore = gapi::AttributeType::ComputeDataSize(m_Attributes, index);
        return m_Data[dataSizeBefore];
    }

    template <>
    float Vertex::Attribute(size_t index) const {
        return const_cast<Vertex*>(this)->Attribute<float>(index);
    }

    template <>
    glm::vec2& Vertex::Attribute(size_t index) {
        CORE_ASSERT(index < m_Attributes.size(), "Requested attribute is not allocated");
        CORE_ASSERT(m_Attributes[index] == gapi::AttributeType::FLOAT_2, "Vertex attribute is not of type Float2!");

        uint64_t dataSizeBefore = gapi::AttributeType::ComputeDataSize(m_Attributes, index);
        return *(reinterpret_cast<glm::vec2*>(&m_Data[dataSizeBefore]));
    }

    template <>
    glm::vec2 Vertex::Attribute(size_t index) const {
        return glm::vec2(const_cast<Vertex*>(this)->Attribute<glm::vec2>(index));
    }

    template <>
    glm::vec3& Vertex::Attribute(size_t index) {
        CORE_ASSERT(index < m_Attributes.size(), "Requested attribute is not allocated");
        CORE_ASSERT(m_Attributes[index] == gapi::AttributeType::FLOAT_3, "Vertex attribute is not of type Float3!");

        uint64_t dataSizeBefore = gapi::AttributeType::ComputeDataSize(m_Attributes, index);
        return *(reinterpret_cast<glm::vec3*>(&m_Data[dataSizeBefore]));
    }

    template <>
    glm::vec3 Vertex::Attribute(size_t index) const {
        return glm::vec3(const_cast<Vertex*>(this)->Attribute<glm::vec3>(index));
    }

    template <>
    glm::vec4& Vertex::Attribute(size_t index) {
        CORE_ASSERT(index < m_Attributes.size(), "Requested attribute is not allocated");
        CORE_ASSERT(m_Attributes[index] == gapi::AttributeType::FLOAT_4, "Vertex attribute is not of type Float4!");

        uint64_t dataSizeBefore = gapi::AttributeType::ComputeDataSize(m_Attributes, index);
        return *(reinterpret_cast<glm::vec4*>(&m_Data[dataSizeBefore]));
    }

    template <>
    glm::vec4 Vertex::Attribute(size_t index) const {
        return glm::vec4(const_cast<Vertex*>(this)->Attribute<glm::vec4>(index));
    }

    const std::vector<float>& Vertex::Data() const {
        return m_Data;
    }

    size_t Vertex::SizeInBytes() const {
        // Vertex contains only floats and a float contains 4 bytes, so the total amount must be equal to size * 4
        return (m_Data.size() << 2);
    }

}
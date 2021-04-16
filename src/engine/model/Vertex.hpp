//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_VERTEX_HPP
#define VD_GAME_ENGINE_VERTEX_HPP

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include <engine/exception/Exceptions.hpp>
#include <engine/gapi/Buffer.hpp>

namespace vd::model {

    class Vertex {
    public:
        Vertex() = default;
        explicit Vertex(const gapi::AttributeTypeVec& attributes);
        explicit Vertex(const std::vector<float>& rawData);

        [[nodiscard]] bool SameAttributes(const gapi::AttributeTypeVec& attributes) const;

        void Assign(const gapi::AttributeTypeVec& attributes);
        void Assign(const std::vector<float>& rawData);

        template <typename T>
        T& Attribute(size_t index);

        template <typename T>
        T Attribute(size_t index) const;

        [[nodiscard]] const std::vector<float>& Data() const;
        [[nodiscard]] size_t SizeInBytes() const;
    private:
        gapi::AttributeTypeVec  m_Attributes;
        std::vector<float>      m_Data;
    };

}

#endif //VD_GAME_ENGINE_VERTEX_HPP

//
// Created by Vali on 9/19/2020.
//

#ifndef VD_GAME_ENGINE_BOUNDS_HPP
#define VD_GAME_ENGINE_BOUNDS_HPP

#include <engine/defines/Macros.hpp>
#include <engine/model/Vertex.hpp>

#include <algorithm>

#include "Transform.hpp"

namespace vd::math {
    template <typename T>
    class Bounds {
    public:
        Bounds();
        Bounds(const T& left, const T& right);
        ~Bounds();

        [[nodiscard]] bool Empty() const;
        [[nodiscard]] bool Valid() const;

        void Validate();

        [[nodiscard]] const T& Left() const;
        [[nodiscard]] const T& Right() const;

    protected:
        T m_Left;
        T m_Right;
    };

    class Bounds2 : public Bounds<glm::vec2> {
    public:
        Bounds2();
        Bounds2(const glm::vec2& left, const glm::vec2& right);
        explicit Bounds2(const std::vector<model::Vertex>& vertices);

        void WrapVertices(const std::vector<model::Vertex>& vertices);
    };
    typedef std::vector<Bounds2>    Bounds2Vec;

    class Bounds3 : public Bounds<glm::vec3> {
    public:
        Bounds3();
        Bounds3(const glm::vec3& left, const glm::vec3& right);
        explicit Bounds3(const std::vector<model::Vertex>& vertices);

        void WrapVertices(const std::vector<model::Vertex>& vertices);

        [[nodiscard]] Bounds3 WithTransform(const Transform& transform) const;
    };
    typedef std::vector<Bounds3>    Bounds3Vec;
}


#endif //VD_GAME_ENGINE_BOUNDS_HPP

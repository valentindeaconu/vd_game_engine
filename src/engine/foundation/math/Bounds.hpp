//
// Created by Vali on 9/19/2020.
//

#ifndef VD_GAME_ENGINE_BOUNDS_HPP
#define VD_GAME_ENGINE_BOUNDS_HPP

#include <engine/model/Mesh.hpp>

#include <algorithm>

#include "Transform.hpp"

namespace vd::math {
    template <typename T>
    class Bounds {
    public:
        Bounds();
        Bounds(const T& left, const T& right);
        ~Bounds();


        [[nodiscard]] bool IsEmpty() const;
        [[nodiscard]] bool IsValid() const;

        [[nodiscard]] const T& GetLeft() const;
        [[nodiscard]] const T& GetRight() const;

    protected:
        T m_Left;
        T m_Right;
    };
    typedef Bounds<glm::vec2>   Bounds2;

    class Bounds3 : public Bounds<glm::vec3> {
    public:
        Bounds3();
        Bounds3(const glm::vec3& left, const glm::vec3& right);
        explicit Bounds3(const model::MeshPtr& meshPtr);

        void wrapMesh(const model::MeshPtr& meshPtr);

        [[nodiscard]] Bounds3 withTransform(const Transform& transform) const;
    };

    typedef std::vector<Bounds3>    Bounds3Vec;
}


#endif //VD_GAME_ENGINE_BOUNDS_HPP

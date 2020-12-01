//
// Created by Vali on 9/19/2020.
//

#include "Bounds.hpp"

namespace vd::math {

    template class Bounds<glm::vec2>;
    template class Bounds<glm::vec3>;

    template<typename T>
    Bounds<T>::Bounds()
        : m_Left(0.0f)
        , m_Right(0.0f)
    {
    }

    template<typename T>
    Bounds<T>::Bounds(const T& left, const T& right)
        : m_Left(left)
        , m_Right(right)
    {
    }

    template<typename T>
    Bounds<T>::~Bounds() = default;

    template<typename T>
    bool Bounds<T>::Empty() const {
        return m_Left == m_Right;
    }

    template<>
    bool Bounds<glm::vec2>::Valid() const {
        return m_Left.x <= m_Right.x && m_Left.y <= m_Right.y;
    }

    template<>
    bool Bounds<glm::vec3>::Valid() const {
        return m_Left.x <= m_Right.x && m_Left.y <= m_Right.y && m_Left.z <= m_Right.z;
    }

    template<typename T>
    const T& Bounds<T>::Left() const {
        return m_Left;
    }

    template<typename T>
    const T& Bounds<T>::Right() const {
        return m_Right;
    }

    Bounds3::Bounds3() : Bounds() { }

    Bounds3::Bounds3(const glm::vec3& left, const glm::vec3&right) : Bounds(left, right) { }

    Bounds3::Bounds3(const model::MeshPtr& meshPtr) : Bounds<glm::vec3>() {
        WrapMesh(meshPtr);
    }

    void Bounds3::WrapMesh(const model::MeshPtr& meshPtr) {
        if (meshPtr != nullptr && !meshPtr->Vertices().empty()) {
            m_Left.x = m_Right.x = meshPtr->Vertices().front().Position.x;
            m_Left.y = m_Right.y = meshPtr->Vertices().front().Position.y;
            m_Left.z = m_Right.z = meshPtr->Vertices().front().Position.z;

            for (size_t i = 1; i < meshPtr->Vertices().size(); ++i) {
                model::Vertex& v = meshPtr->Vertices()[i];

                m_Right.x = std::max(m_Right.x, v.Position.x);
                m_Left.x = std::min(m_Left.x, v.Position.x);

                m_Right.y = std::max(m_Right.y, v.Position.y);
                m_Left.y = std::min(m_Left.y, v.Position.y);

                m_Right.z = std::max(m_Right.z, v.Position.z);
                m_Left.z = std::min(m_Left.z, v.Position.z);
            }
        }
    }

    Bounds3 Bounds3::WithTransform(const Transform& transform) const {
        return Bounds3(
            glm::vec3(transform.Get() * glm::vec4(m_Left, 1.0f)),
            glm::vec3(transform.Get() * glm::vec4(m_Right, 1.0f))
        );
    }

}
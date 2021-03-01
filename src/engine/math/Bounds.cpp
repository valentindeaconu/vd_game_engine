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

    template<>
    void Bounds<glm::vec2>::Validate() {
        float x_min = std::min(m_Left.x, m_Right.x);
        float x_max = std::max(m_Left.x, m_Right.x);

        float y_min = std::min(m_Left.y, m_Right.y);
        float y_max = std::max(m_Left.y, m_Right.y);

        m_Left = glm::vec2(x_min, y_min);
        m_Right = glm::vec2(x_max, y_max);
    }

    template<>
    void Bounds<glm::vec3>::Validate() {
        float x_min = std::min(m_Left.x, m_Right.x);
        float x_max = std::max(m_Left.x, m_Right.x);

        float y_min = std::min(m_Left.y, m_Right.y);
        float y_max = std::max(m_Left.y, m_Right.y);

        float z_min = std::min(m_Left.z, m_Right.z);
        float z_max = std::max(m_Left.z, m_Right.z);

        m_Left = glm::vec3(x_min, y_min, z_min);
        m_Right = glm::vec3(x_max, y_max, z_max);
    }

    template<typename T>
    const T& Bounds<T>::Left() const {
        return m_Left;
    }

    template<typename T>
    const T& Bounds<T>::Right() const {
        return m_Right;
    }


    Bounds2::Bounds2() : Bounds<glm::vec2>() { }

    Bounds2::Bounds2(const glm::vec2 &left, const glm::vec2 &right) : Bounds<glm::vec2>(left, right) { }

    Bounds2::Bounds2(const model::Mesh2DPtr& meshPtr) : Bounds<glm::vec2>() {
        WrapMesh(meshPtr);
    }

    void Bounds2::WrapMesh(const model::Mesh2DPtr& meshPtr) {
        if (meshPtr != nullptr && !meshPtr->Vertices().empty()) {
            m_Left.x = m_Right.x = meshPtr->Vertices().front().Position.x;
            m_Left.y = m_Right.y = meshPtr->Vertices().front().Position.y;

            for (size_t i = 1; i < meshPtr->Vertices().size(); ++i) {
                model::Vertex2D& v = meshPtr->Vertices()[i];

                m_Right.x = std::max(m_Right.x, v.Position.x);
                m_Left.x = std::min(m_Left.x, v.Position.x);

                m_Right.y = std::max(m_Right.y, v.Position.y);
                m_Left.y = std::min(m_Left.y, v.Position.y);
            }
        }
    }

    Bounds3::Bounds3() : Bounds() { }

    Bounds3::Bounds3(const glm::vec3& left, const glm::vec3&right) : Bounds(left, right) { }

    Bounds3::Bounds3(const model::Mesh3DPtr& meshPtr) : Bounds<glm::vec3>() {
        WrapMesh(meshPtr);
    }

    void Bounds3::WrapMesh(const model::Mesh3DPtr& meshPtr) {
        if (meshPtr != nullptr && !meshPtr->Vertices().empty()) {
            m_Left.x = m_Right.x = meshPtr->Vertices().front().Position.x;
            m_Left.y = m_Right.y = meshPtr->Vertices().front().Position.y;
            m_Left.z = m_Right.z = meshPtr->Vertices().front().Position.z;

            for (size_t i = 1; i < meshPtr->Vertices().size(); ++i) {
                model::Vertex3D& v = meshPtr->Vertices()[i];

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
        Bounds3 transformedBounds(glm::vec3(transform.Get() * glm::vec4(m_Left, 1.0f)),
                                  glm::vec3(transform.Get() * glm::vec4(m_Right, 1.0f)));
        if (!transformedBounds.Valid()) {
            transformedBounds.Validate();
        }

        return transformedBounds;
    }

}
//
// Created by Vali on 8/26/2020.
//

#include "ShadowBox.hpp"

namespace mod::shadow {
    ShadowBox::ShadowBox(std::shared_ptr<glm::mat4> viewMatrixPtr, float distance, float offset)
        : m_ViewMatrixPtr(std::move(viewMatrixPtr))
        , m_kDistance(distance)
        , m_kOffset(offset)
        , m_X({ .min = 0.0f, .max = 0.0f })
        , m_Y({ .min = 0.0f, .max = 0.0f })
        , m_Z({ .min = 0.0f, .max = 0.0f })
    {
    }

    ShadowBox::~ShadowBox() = default;

    void ShadowBox::Link() {
        vd::window::WindowPtr windowPtr = vd::ObjectOfType<vd::window::Window>::Find();
        m_AspectRatioGetter = [w = windowPtr.get()] { return w->AspectRatio(); };
        m_NearPlaneGetter = [w = windowPtr.get()] { return w->NearPlane(); };
        m_FieldOfViewGetter = [w = windowPtr.get()] { return w->FieldOfView(); };
        m_PerspectiveChangedChecker = [w = windowPtr.get()] { return w->PerspectiveChanged(); };

        vd::camera::CameraPtr cameraPtr = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_CameraPositionGetter = [c = cameraPtr.get()] { return c->Position(); };
        m_CameraForwardGetter = [c = cameraPtr.get()] { return c->Forward(); };
        m_CameraRightGetter = [c = cameraPtr.get()] { return c->Right(); };
        m_CameraUpGetter = [c = cameraPtr.get()] { return c->Up(); };

        UpdateWidthsAndHeights();
    }

    void ShadowBox::Update() {
        std::vector<glm::vec4> points = FrustumVertices();

        m_X.min = m_X.max = points.front().x;
        m_Y.min = m_Y.max = points.front().y;
        m_Z.min = m_Z.max = points.front().z;

        for (int i = 1; i < points.size(); ++i) {
            m_X.max = std::max(m_X.max, points[i].x);
            m_X.min = std::min(m_X.min, points[i].x);

            m_Y.max = std::max(m_Y.max, points[i].y);
            m_Y.min = std::min(m_Y.min, points[i].y);

            m_Z.max = std::max(m_Z.max, points[i].z);
            m_Z.min = std::min(m_Z.min, points[i].z);
        }

        m_Z.max += m_kOffset;
    }

    glm::vec3 ShadowBox::Center() const {
        glm::vec4 center;

        center.x = (m_X.min + m_X.max) / 2.0f;
        center.y = (m_Y.min + m_Y.max) / 2.0f;
        center.z = (m_Z.min + m_Z.max) / 2.0f;
        center.w = 1.0f;

        // center coordinates are in light space, so convert them back to world space by multiplying with
        // inverse of lightView matrix
        return glm::vec3(glm::inverse(*m_ViewMatrixPtr) * center);
    }

    const ShadowBox::Coordinates& ShadowBox::X() const {
        return m_X;
    }

    const ShadowBox::Coordinates& ShadowBox::Y() const {
        return m_Y;
    }

    const ShadowBox::Coordinates& ShadowBox::Z() const {
        return m_Z;
    }

    std::vector<glm::vec4> ShadowBox::FrustumVertices() {
        glm::vec3 cameraPosition = m_CameraPositionGetter();
        glm::vec3 forward = m_CameraForwardGetter();
        glm::vec3 right = m_CameraRightGetter();
        glm::vec3 up = m_CameraUpGetter();

        float zNear = m_NearPlaneGetter();

        // Compute center points
        glm::vec3 fc = cameraPosition + (forward * m_kDistance); // far plane center point
        glm::vec3 nc = cameraPosition + (forward * zNear); // near plane center point

        if (m_PerspectiveChangedChecker()) {
            UpdateWidthsAndHeights();
        }

        return std::vector<glm::vec4>({
            *m_ViewMatrixPtr * glm::vec4(nc - up * m_Near.height - right * m_Near.width, 1.0f), // near bottom left
            *m_ViewMatrixPtr * glm::vec4(nc + up * m_Near.height - right * m_Near.width, 1.0f), // near top left
            *m_ViewMatrixPtr * glm::vec4(nc + up * m_Near.height + right * m_Near.width, 1.0f), // near top right
            *m_ViewMatrixPtr * glm::vec4(nc - up * m_Near.height + right * m_Near.width, 1.0f), // near bottom right

            *m_ViewMatrixPtr * glm::vec4(fc - up * m_Far.height - right * m_Far.width, 1.0f), // far bottom left
            *m_ViewMatrixPtr * glm::vec4(fc + up * m_Far.height - right * m_Far.width, 1.0f), // far top left
            *m_ViewMatrixPtr * glm::vec4(fc + up * m_Far.height + right * m_Far.width, 1.0f), // far top right
            *m_ViewMatrixPtr * glm::vec4(fc - up * m_Far.height + right * m_Far.width, 1.0f) // far bottom right
        });
    }

    void ShadowBox::UpdateWidthsAndHeights() {
        // these heights and widths are half the heights and widths of
        // the near and far plane rectangles

        float zNear = m_NearPlaneGetter();
        float fovY = m_FieldOfViewGetter();
        float aspect = m_AspectRatioGetter();

        m_Far.width = m_kDistance * std::tan(glm::radians(fovY));
        m_Near.width = zNear * std::tan(glm::radians(fovY));
        m_Far.height = m_Far.width / aspect;
        m_Near.height = m_Near.width / aspect;
    }
}



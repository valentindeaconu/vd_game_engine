//
// Created by Vali on 11/11/2020.
//

#include "FrustumCullingManager.hpp"

namespace vd::culling {

    FrustumCullingManager::FrustumCullingManager()
        : m_Far()
        , m_Near()
    {
    }

    FrustumCullingManager::~FrustumCullingManager() = default;

    void FrustumCullingManager::Link() {
        m_pCamera = ObjectOfType<camera::Camera>::Find();
        m_pWindow = ObjectOfType<window::Window>::Find();
    }

    void FrustumCullingManager::Init() {
        UpdateVertices();
        UpdatePlanes();
    }

    void FrustumCullingManager::Update() {
        // TODO: As optimization, this function should be called only if camera perspective is
        //  changed or window is resized

        UpdateVertices();
        UpdatePlanes();
    }

    void FrustumCullingManager::CleanUp() {

    }

    const vd::math::Frustum &FrustumCullingManager::Frustum() const {
        return m_Frustum;
    }

    void FrustumCullingManager::UpdateVertices() {
        glm::vec3 cameraPosition = m_pCamera->Position();
        glm::vec3 forward = m_pCamera->Forward();
        glm::vec3 right = m_pCamera->Right();
        glm::vec3 up = m_pCamera->Up();

        float zNear = m_pWindow->NearPlane();
        float zFar = m_pWindow->FarPlane();

        // Compute center points
        glm::vec3 fc = cameraPosition + (forward * zFar); // far plane center point
        glm::vec3 nc = cameraPosition + (forward * zNear); // near plane center point

        if (m_pWindow->PerspectiveChanged()) {
            UpdateWidthsAndHeights();
        }

        using vd::math::Frustum;
        Frustum::VertexVec vertices(Frustum::kVertexCount);

        vertices[Frustum::Vertex::eNearBottomLeft] = nc - up * m_Near.height - right * m_Near.width;
        vertices[Frustum::Vertex::eNearBottomRight] = nc - up * m_Near.height + right * m_Near.width;
        vertices[Frustum::Vertex::eNearTopLeft] = nc + up * m_Near.height - right * m_Near.width;
        vertices[Frustum::Vertex::eNearTopRight] = nc + up * m_Near.height + right * m_Near.width;
        vertices[Frustum::Vertex::eFarBottomLeft] = fc - up * m_Far.height - right * m_Far.width;
        vertices[Frustum::Vertex::eFarBottomRight] = fc - up * m_Far.height + right * m_Far.width;
        vertices[Frustum::Vertex::eFarTopLeft] = fc + up * m_Far.height - right * m_Far.width;
        vertices[Frustum::Vertex::eFarTopRight] = fc + up * m_Far.height + right * m_Far.width;

        m_Frustum.Vertices() = vertices;
    }

    void FrustumCullingManager::UpdatePlanes() {
        using vd::math::Frustum;
        using vd::math::Plane;
        using vd::math::PlaneVec;

        auto &vertices = m_Frustum.Vertices();

        PlaneVec planes(Frustum::kPlaneCount);

        planes[Frustum::Plane::eLeft] = Plane(vertices[Frustum::Vertex::eFarBottomLeft],
                                              vertices[Frustum::Vertex::eNearBottomLeft],
                                              vertices[Frustum::Vertex::eFarTopLeft],
                                              vertices[Frustum::Vertex::eNearTopLeft]);

        planes[Frustum::Plane::eRight] = Plane(vertices[Frustum::Vertex::eNearBottomRight],
                                               vertices[Frustum::Vertex::eFarBottomRight],
                                               vertices[Frustum::Vertex::eNearTopRight],
                                               vertices[Frustum::Vertex::eFarTopRight]);

        planes[Frustum::Plane::eNear] = Plane(vertices[Frustum::Vertex::eNearBottomLeft],
                                              vertices[Frustum::Vertex::eNearBottomRight],
                                              vertices[Frustum::Vertex::eNearTopLeft],
                                              vertices[Frustum::Vertex::eNearTopRight]);

        planes[Frustum::Plane::eFar] = Plane(vertices[Frustum::Vertex::eFarBottomRight],
                                             vertices[Frustum::Vertex::eFarBottomLeft],
                                             vertices[Frustum::Vertex::eFarTopRight],
                                             vertices[Frustum::Vertex::eFarTopLeft]);

        planes[Frustum::Plane::eTop] = Plane(vertices[Frustum::Vertex::eNearTopLeft],
                                             vertices[Frustum::Vertex::eNearTopRight],
                                             vertices[Frustum::Vertex::eFarTopLeft],
                                             vertices[Frustum::Vertex::eFarTopRight]);

        planes[Frustum::Plane::eBottom] = Plane(vertices[Frustum::Vertex::eFarBottomLeft],
                                                vertices[Frustum::Vertex::eFarBottomRight],
                                                vertices[Frustum::Vertex::eNearBottomLeft],
                                                vertices[Frustum::Vertex::eNearBottomRight]);

        m_Frustum.Planes() = planes;
    }

    void FrustumCullingManager::UpdateWidthsAndHeights() {
        float zNear = m_pWindow->NearPlane();
        float zFar = m_pWindow->FarPlane();
        float fovY = m_pWindow->FieldOfView();
        float aspect = m_pWindow->AspectRatio();

        m_Far.width = zFar * std::tan(glm::radians(fovY));
        m_Near.width = zNear * std::tan(glm::radians(fovY));
        m_Far.height = m_Far.width / aspect;
        m_Near.height = m_Near.width / aspect;
    }
}
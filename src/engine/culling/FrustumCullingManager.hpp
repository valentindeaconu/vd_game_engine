//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_FRUSTUMCULLINGMANAGER_HPP
#define VD_GAME_ENGINE_FRUSTUMCULLINGMANAGER_HPP

#include <engine/component/IManager.hpp>

#include <engine/math/Plane.hpp>
#include <engine/math/Frustum.hpp>

#include <engine/misc/ObjectOfType.hpp>

#include <memory>

namespace vd::culling {
    class FrustumCullingManager : public vd::component::IManager {
    public:
        FrustumCullingManager();
        ~FrustumCullingManager();

        void Init() override;
        void Update() override;
        void CleanUp() override;

        [[nodiscard]] const vd::math::Frustum& GetFrustum() const;
    private:
        void UpdateVertices();
        void UpdatePlanes();
        void UpdateWidthsAndHeights();

        vd::math::Frustum m_Frustum;

        struct { float width, height; } m_Near, m_Far;

        window::WindowPtr m_WindowPtr;
        camera::CameraPtr m_CameraPtr;
    };
    typedef std::shared_ptr<FrustumCullingManager>  FrustumCullingManagerPtr;
}

#endif //VD_GAME_ENGINE_FRUSTUMCULLINGMANAGER_HPP

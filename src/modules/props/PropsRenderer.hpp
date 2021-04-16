//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_PROPRENDERER_HPP
#define VD_GAME_ENGINE_PROPRENDERER_HPP

#include <engine/component/IRenderer.hpp>
#include <modules/shadow/ShadowShader.hpp>

#include <engine/injector/Injectable.hpp>

#include <engine/collision/Detector.hpp>
#include <engine/culling/FrustumCullingManager.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/event/EventHandler.hpp>
#include <modules/terrain/Terrain.hpp>
#include <modules/biomes/BiomeManager.hpp>

#include "PropsManager.hpp"

namespace mod::props {
    class PropsRenderer
            : public vd::component::IRenderer
            , public vd::injector::Injectable {
    public:
        static const vd::datastruct::Observable::priority_t kPriority = biomes::BiomeManager::kPriority + 1;

        explicit PropsRenderer(PropsManagerPtr propsManager, vd::component::IEntityShaderPtr shader);

        void Link() override;

        void OnInit() override;
        void OnUpdate() override;
        void OnRender(const params_t& params) override;
        void OnCleanUp() override;
    private:
        bool Precondition(const params_t& params) override;
        void Prepare() override;
        void Finish() override;

        struct Props {
            std::vector<vd::math::Transform>    Transforms;
            std::vector<PropPtr>                Props;
            std::vector<uint64_t>               Levels;
            std::vector<bool>                   Culled;
            size_t                              Total;
        } m_Units;

        PropsManagerPtr                         m_pPropsManager;
        vd::component::IEntityShaderPtr         m_pShader;
        vd::camera::CameraPtr                   m_pCamera;
        vd::culling::FrustumCullingManagerPtr   m_pFrustumCullingManager;
        mod::shadow::ShadowShaderPtr            m_pShadowShader;
    };
    typedef std::shared_ptr<PropsRenderer>	PropsRendererPtr;
}

#endif //VD_GAME_ENGINE_PROPRENDERER_HPP

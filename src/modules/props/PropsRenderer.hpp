//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_PROPRENDERER_HPP
#define VD_GAME_ENGINE_PROPRENDERER_HPP

#include <engine/component/IRenderer.hpp>

#include <engine/injector/Injectable.hpp>

#include <engine/collision/Detector.hpp>
#include <engine/culling/FrustumCullingManager.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/event/EventHandler.hpp>
#include <modules/shadow/ShadowShader.hpp>
#include <modules/terrain/Terrain.hpp>

#include "PropGenerator.hpp"

namespace mod::props {
    class PropsRenderer
            : public vd::component::IRenderer
            , public vd::injector::Injectable {
    public:
        explicit PropsRenderer(vd::component::IEntityShaderPtr shaderPtr,
                      vd::Consumer beforeExecution = vd::g_kEmptyConsumer,
                      vd::Consumer afterExecution = vd::g_kEmptyConsumer);
        ~PropsRenderer();

        void Link() override;

        void Init() override;
        void Update() override;
        void Render(const params_t& params) override;
        void CleanUp() override;
    private:
        bool IsReady() override;

        struct Props {
            std::vector<vd::math::Transform>    Transforms;
            std::vector<PropPtr>                Props;
            std::vector<uint64_t>               Levels;
            std::vector<bool>                   Culled;
            size_t                              Total;
        } m_Units;

        PropGeneratorPtr m_pPropGenerator;

        vd::event::EventHandlerPtr              m_pEventHandler;
        vd::camera::CameraPtr                   m_pCamera;
        vd::culling::FrustumCullingManagerPtr   m_pFrustumCullingManager;
        mod::shadow::ShadowShaderPtr            m_pShadowShader;
    };
    typedef std::shared_ptr<PropsRenderer>	PropsRendererPtr;
}

#endif //VD_GAME_ENGINE_PROPRENDERER_HPP

//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_PROPRENDERER_HPP
#define VD_GAME_ENGINE_PROPRENDERER_HPP

#include <engine/component/IRenderer.hpp>

#include <engine/injector/Injectable.hpp>

#include <engine/collision/Detector.hpp>
#include <engine/culling/FrustumCullingManager.hpp>
#include <modules/shadow/ShadowShader.hpp>
#include <modules/terrain/Terrain.hpp>

#include "PropGenerator.hpp"

namespace mod::props {
    class PropsRenderer
            : public vd::component::IRenderer
            , public vd::injector::Injectable {
    public:
        explicit PropsRenderer(vd::component::EntityShaderPtr shaderPtr,
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

        PropGeneratorPtr m_pPropGenerator;

        mod::shadow::ShadowShaderPtr            m_pShadowShader;
        vd::culling::FrustumCullingManagerPtr   m_pFrustumCullingManager;
    };
    typedef std::shared_ptr<PropsRenderer>	PropsRendererPtr;
}

#endif //VD_GAME_ENGINE_PROPRENDERER_HPP

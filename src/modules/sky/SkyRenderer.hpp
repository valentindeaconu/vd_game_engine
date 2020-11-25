//
// Created by Vali on 11/12/2020.
//

#ifndef VD_GAME_ENGINE_SKYRENDERER_HPP
#define VD_GAME_ENGINE_SKYRENDERER_HPP

#include <engine/component/IRenderer.hpp>

#include <engine/injector/Injectable.hpp>
#include <modules/shadow/ShadowShader.hpp>

#include <string>

#include "Sky.hpp"

namespace mod::sky {
    class SkyRenderer : public vd::component::IRenderer, public vd::injector::Injectable {
    public:
        SkyRenderer(SkyPtr pSky,
                    vd::gl::EntityShaderPtr pShader,
                    vd::Consumer beforeExecution = vd::g_kEmptyConsumer,
                    vd::Consumer afterExecution = vd::g_kEmptyConsumer);
        ~SkyRenderer();

        void Link() override;

        void Init() override;
        void Update() override;
        void Render(const params_t& params) override;
        void CleanUp() override;

    private:
        bool IsReady() override;

        SkyPtr m_pSky;

        mod::shadow::ShadowShaderPtr m_pShadowShader;
    };

    typedef std::shared_ptr<SkyRenderer>	SkyRendererPtr;
}

#endif //VD_GAME_ENGINE_SKYRENDERER_HPP
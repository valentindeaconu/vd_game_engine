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
        SkyRenderer(SkyPtr sky, vd::component::IEntityShaderPtr shader);

        void Link() override;

        void OnInit() override;
        void OnUpdate() override;
        void OnRender(const params_t& params) override;
        void OnCleanUp() override;

    private:
        bool Precondition(const params_t& params) override;
        void Prepare();
        void Finish();

        SkyPtr                          m_pSky;
        vd::component::IEntityShaderPtr m_pShader;
        mod::shadow::ShadowShaderPtr    m_pShadowShader;
    };

    typedef std::shared_ptr<SkyRenderer>	SkyRendererPtr;
}

#endif //VD_GAME_ENGINE_SKYRENDERER_HPP
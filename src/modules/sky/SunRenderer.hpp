//
// Created by Vali on 4/6/2021.
//

#ifndef VDGE_SUNRENDERER_HPP
#define VDGE_SUNRENDERER_HPP

#include <memory>

#include <engine/component/IRenderer.hpp>
#include <engine/component/IEntityShader.hpp>

#include "Sun.hpp"

namespace mod::sky {
    class SunRenderer : public vd::component::IRenderer {
    public:
        SunRenderer(SunPtr sun, vd::component::IEntityShaderPtr shader);

        void OnInit() override;
        void OnUpdate() override;
        void OnRender(const params_t& params) override;
        void OnCleanUp() override;

    private:
        bool Precondition(const params_t& params) override;
        void Prepare() override;
        void Finish() override;

        SunPtr                          m_pSun;
        vd::component::IEntityShaderPtr m_pShader; 
    };
    typedef std::shared_ptr<SunRenderer>	SunRendererPtr;
}

#endif //VDGE_SUNRENDERER_HPP
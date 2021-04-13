//
// Created by Vali on 2/26/2021.
//

#ifndef VDGE_EFFECTRENDERER_HPP
#define VDGE_EFFECTRENDERER_HPP

#include <engine/component/IRenderer.hpp>
#include <engine/object/primitive/Quad2D.hpp>

#include <engine/component/RenderingEffect.hpp>
#include <engine/component/IRenderingEffectShader.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/window/Window.hpp>

namespace mod::postprocessing {
    class EffectRenderer : public vd::component::IRenderer, public vd::injector::Injectable {
    public:
        static const int kPriority = kDefaultPriority + 99;

        EffectRenderer();

        void Link() override;

        void OnInit() override;
        void OnUpdate() override;
        void OnRender(const params_t& params) override;
        void OnCleanUp() override;

        void PushStage(const vd::component::IRenderingEffectPtr& pEffect,
                       const vd::component::IRenderingEffectShaderPtr& pShader);
    private:
        bool Precondition(const params_t& params) override;
        void Prepare();
        void Finish();

        struct Stage {
            vd::component::IRenderingEffectPtr          Effect;
            vd::component::IRenderingEffectShaderPtr    Shader;
        };
        std::vector<Stage>                  m_Stages;
        vd::object::primitive::Quad2DPtr    m_pQuad;

        // Utility
        vd::window::WindowPtr   m_pWindow;
    };
    typedef std::shared_ptr<EffectRenderer>	EffectRendererPtr;
}


#endif //VDGE_EFFECTRENDERER_HPP

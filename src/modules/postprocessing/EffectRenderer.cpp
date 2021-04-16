//
// Created by Vali on 2/26/2021.
//

#include "EffectRenderer.hpp"

namespace mod::postprocessing {

    EffectRenderer::EffectRenderer()
        : vd::component::IRenderer("EffectRenderer")
    {
        m_pQuad = std::make_shared<vd::object::primitive::Quad2D>();
    }

    void EffectRenderer::Link() {
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
    }

    void EffectRenderer::OnInit() {
        m_pQuad->Init();

        vd::Dimension windowDimension(m_pWindow->Width(), m_pWindow->Height());

        for (auto& s : m_Stages) {
            s.Effect->Init(windowDimension);

            s.Shader->Bind();
            s.Shader->InitUniforms(s.Effect);
            s.Shader->Unbind();
        }
    }

    void EffectRenderer::OnUpdate() {
        m_pQuad->Update();
    }

    void EffectRenderer::OnRender(const vd::datastruct::Observer::params_t& params) {
        for (auto& s : m_Stages) {
            vd::component::IRenderingEffectPtr& effect = s.Effect;
            vd::component::IRenderingEffectShaderPtr& shader = s.Shader;

            if (effect->Precondition()) {
                effect->Prepare();

                shader->Bind();
                shader->UpdateUniforms(effect);

                // m_pQuad->Buffers()[0]->DrawArrays(vd::gl::eTriangleStrip, 4);
                m_pQuad->Meshes()[0]->Draw();

                shader->Unbind();

                effect->Finish();
            }
        }
    }

    void EffectRenderer::OnCleanUp() {
        m_pQuad->CleanUp();

        for (auto& s : m_Stages) {
            s.Effect->CleanUp();
            s.Shader->CleanUp();
        }

        m_Stages.clear();
    }

    void EffectRenderer::PushStage(const vd::component::IRenderingEffectPtr& pEffect,
                                   const vd::component::IRenderingEffectShaderPtr& pShader) {
        Stage s = { pEffect, pShader };
        m_Stages.emplace_back(std::move(s));
    }

    bool EffectRenderer::Precondition(const params_t& params) {
        if (m_Stages.empty()) {
            return false;
        }
        
        return params.at("RenderingPass") == "PostProcessing";
    }

    void EffectRenderer::Prepare() {
        vd::gl::Context::CounterClockwiseFacing();
    }

    void EffectRenderer::Finish() {
        vd::gl::Context::Reset();
    }
}
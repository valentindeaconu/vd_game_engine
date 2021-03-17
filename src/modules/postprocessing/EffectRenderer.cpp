//
// Created by Vali on 2/26/2021.
//

#include "EffectRenderer.hpp"

namespace mod::postprocessing {

    EffectRenderer::EffectRenderer()
        : vd::component::IRenderer(nullptr,
                                   []() { glFrontFace(GL_CCW); },
                                   []() { glFrontFace(GL_CW); })
    {
        m_pQuad = std::make_shared<vd::object::primitive::Quad2D>();
    }

    void EffectRenderer::Link() {
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
    }

    void EffectRenderer::Init() {
        m_pQuad->Init();

        vd::Dimension windowDimension(m_pWindow->Width(), m_pWindow->Height());

        for (auto& s : m_Stages) {
            s.Effect->Init(windowDimension);

            s.Shader->Bind();
            s.Shader->InitUniforms(s.Effect);
            s.Shader->Unbind();
        }
    }

    void EffectRenderer::Update() {
        m_pQuad->Update();
    }

    void EffectRenderer::Render(const vd::datastruct::Observer::params_t& params) {
        if (!IsReady()) {
            vd::Logger::warn("EffectRenderer was not ready to render");
            return;
        }

        if (params.at("RenderingPass") == "PostProcessing") {
            Prepare();

            for (auto& s : m_Stages) {
                vd::component::IRenderingEffectPtr& effect = s.Effect;
                vd::component::IRenderingEffectShaderPtr& shader = s.Shader;

                if (effect->Precondition()) {
                    effect->Prepare();

                    shader->Bind();
                    shader->UpdateUniforms(effect);

                    //m_pQuad->Buffers()[0]->DrawElements(vd::gl::eTriangles, 6, vd::gl::eUnsignedInt);
                    m_pQuad->Buffers()[0]->DrawArrays(vd::gl::eTriangleStrip, 4);

                    shader->Unbind();

                    effect->Finish();
                }
            }

            Finish();
        }
    }

    void EffectRenderer::CleanUp() {
        m_pQuad->CleanUp();

        for (auto& s : m_Stages) {
            s.Effect->CleanUp();
            s.Shader->CleanUp();
        }

        m_Stages.clear();
    }

    bool EffectRenderer::IsReady() {
        return !m_Stages.empty();
    }

    void EffectRenderer::PushStage(const vd::component::IRenderingEffectPtr& pEffect,
                                   const vd::component::IRenderingEffectShaderPtr& pShader) {
        Stage s = { pEffect, pShader };
        m_Stages.emplace_back(std::move(s));
    }

}
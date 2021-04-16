//
// Created by Vali on 9/11/2020.
//

#include "WaterRenderer.hpp"

namespace mod::water {

    WaterRenderer::WaterRenderer(WaterPtr waterPtr, vd::component::IEntityShaderPtr shaderPtr)
        : IRenderer("WaterRenderer")
        , m_pShader(std::move(shaderPtr))
        , m_pWater(std::move(waterPtr))
    {
    }

    void WaterRenderer::OnInit() {
        m_pWater->Init();

        m_pShader->Init();
        m_pShader->Bind();
        m_pShader->InitUniforms(m_pWater);
        m_pShader->Unbind();
    }

    void WaterRenderer::OnUpdate() {
        m_pWater->Update();
    }

    void WaterRenderer::OnRender(const params_t& params) {
        m_pShader->Bind();
        m_pShader->UpdateUniforms(m_pWater, 0, 0);

        m_pWater->Meshes(0)[0]->Draw();
        // m_pWater->Buffers()[0]->DrawElements(vd::gl::eTriangles, 6, vd::gl::eUnsignedInt);

        m_pShader->Unbind();
    }

    void WaterRenderer::OnCleanUp() {
        m_pWater->CleanUp();
        m_pShader->CleanUp();
    }

    WaterPtr& WaterRenderer::Water() {
        return m_pWater;
    }

    bool WaterRenderer::Precondition(const params_t& params) {
        if (m_pWater == nullptr || m_pShader == nullptr) {
            return false;
        }
        return params.at("RenderingPass") == "Main";
    }

    void WaterRenderer::Prepare() {
        vd::gl::Context::CounterClockwiseFacing();
        vd::gl::Context::AlphaBlending();
    }

    void WaterRenderer::Finish() {
        vd::gl::Context::Reset();
    }
}
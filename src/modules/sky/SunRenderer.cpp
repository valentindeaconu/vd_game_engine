//
// Created by Vali on 4/6/2021.
//

#include "SunRenderer.hpp"

namespace mod::sky {

    SunRenderer::SunRenderer(SunPtr sun, vd::component::IEntityShaderPtr shader)
        : IRenderer("SunRenderer")
        , m_pShader(std::move(shader))
        , m_pSun(std::move(sun))
    {
    }

    void SunRenderer::OnInit() {
        m_pSun->Init();

        m_pShader->Init();
        m_pShader->Bind();
        m_pShader->InitUniforms(m_pSun);
        m_pShader->Unbind();
    }

    void SunRenderer::OnUpdate() {
        m_pSun->Update();
    }

    void SunRenderer::OnRender(const vd::datastruct::Observer::params_t &params) {
        m_pShader->Bind();

        vd::gl::BufferPtrVec& buffers = m_pSun->Buffers();

        auto& meshes = m_pSun->Meshes();
        m_pShader->UpdateUniforms(m_pSun, 0, 0);
        buffers[0]->DrawArrays(vd::gl::ePoints, 1);

        m_pShader->Unbind();
    }

    void SunRenderer::OnCleanUp() {
        m_pSun->CleanUp();
        m_pShader->CleanUp();
    }

    bool SunRenderer::Precondition(const params_t& params) {
        if (m_pShader == nullptr || m_pSun == nullptr) {
            return false;
        }

        return (params.at("RenderingPass") == "Main");
    }

    void SunRenderer::Prepare() {
        vd::gl::Context::DepthMask(false);
        vd::gl::Context::CounterClockwiseFacing();
        vd::gl::Context::AlphaBlending();
    }

    void SunRenderer::Finish() {
        vd::gl::Context::Reset();
    }
}
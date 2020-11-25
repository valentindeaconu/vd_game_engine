//
// Created by Vali on 11/11/2020.
//

#include "SkyRenderer.hpp"

namespace mod::sky {
    SkyRenderer::SkyRenderer(SkyPtr pSky,
                             vd::gl::EntityShaderPtr pShader,
                             vd::Consumer beforeExecution,
                             vd::Consumer afterExecution)
        : IRenderer(std::move(pShader), std::move(beforeExecution), std::move(afterExecution))
        , m_pSky(std::move(pSky))
    {
    }

    SkyRenderer::~SkyRenderer() = default;

    void SkyRenderer::Link() {
        m_pShadowShader = vd::ObjectOfType<mod::shadow::ShadowShader>::Find();
    }

    void SkyRenderer::Init() {
        m_pSky->Init();

        m_pShader->Bind();
        m_pShader->InitUniforms(m_pSky);
    }

    void SkyRenderer::Update() {
        m_pSky->Update();
    }

    void SkyRenderer::Render(const params_t& params) {
        if (!IsReady()) {
            vd::Logger::warn("SkyRenderer was not ready to render");
            return;
        }

        const auto& renderingPass = params.at("RenderingPass");

        Prepare();

        const vd::gl::EntityShaderPtr& shaderPtr = (renderingPass == "Shadow") ? m_pShadowShader : m_pShader;

        shaderPtr->Bind();

        vd::gl::BufferPtrVec& buffers = m_pSky->Buffers();
        for (size_t meshIndex = 0; meshIndex < buffers.size(); ++meshIndex) {
            shaderPtr->UpdateUniforms(m_pSky, meshIndex);
            buffers[meshIndex]->Render();
        }

        Finish();
    }

    void SkyRenderer::CleanUp() {
        m_pSky->CleanUp();
    }

    bool SkyRenderer::IsReady() {
        return IRenderer::IsReady() && m_pSky != nullptr;
    }
}
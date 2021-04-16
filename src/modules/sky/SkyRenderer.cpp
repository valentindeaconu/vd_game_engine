//
// Created by Vali on 11/11/2020.
//

#include "SkyRenderer.hpp"

namespace mod::sky {
    SkyRenderer::SkyRenderer(SkyPtr pSky, vd::component::IEntityShaderPtr pShader)
        : IRenderer("SkyRenderer")
        , m_pShader(std::move(pShader))
        , m_pSky(std::move(pSky))
    {
    }

    void SkyRenderer::Link() {
        m_pShadowShader = vd::ObjectOfType<mod::shadow::ShadowShader>::Find();
    }

    void SkyRenderer::OnInit() {
        m_pSky->Init();

        m_pShader->Init();
        m_pShader->Bind();
        m_pShader->InitUniforms(m_pSky);
        m_pShader->Unbind();
    }

    void SkyRenderer::OnUpdate() {
        m_pSky->Update();
    }

    void SkyRenderer::OnRender(const params_t& params) {
        const vd::component::IEntityShaderPtr& pShader = (params.at("RenderingPass") == "Shadow") ? m_pShadowShader : m_pShader;

        pShader->Bind();

        const auto levelOfDetail = m_pSky->LevelOfDetailAtDistance(.0f);

        auto& meshes = m_pSky->Meshes(levelOfDetail);
        auto& bufferIndices = m_pSky->BufferIndices(levelOfDetail);

        // vd::gl::BufferPtrVec& buffers = m_pSky->Buffers();
        for (size_t meshIndex = 0; meshIndex < meshes.size(); ++meshIndex) {
            pShader->UpdateUniforms(m_pSky, levelOfDetail, meshIndex);
            //buffers[ bufferIndices[meshIndex] ]->DrawElements(vd::gl::eTriangles, 36, vd::gl::eUnsignedInt);
            meshes[meshIndex]->Draw();
        }

        pShader->Unbind();
    }

    void SkyRenderer::OnCleanUp() {
        m_pSky->CleanUp();
        m_pShader->CleanUp();
    }

    bool SkyRenderer::Precondition(const params_t& params) {
        if (m_pSky == nullptr || m_pShader == nullptr) {
            return false;
        }

        const auto& renderingPass = params.at("RenderingPass");

        // TODO: Is sky necessary to be drawn on Shadow Rendering Pass?
        return (renderingPass == "Shadow" ||
                renderingPass == "Reflection" ||
                renderingPass == "Refraction" ||
                renderingPass == "Main");
    }

    void SkyRenderer::Prepare() {
        vd::gl::Context::LequalDepthTesting();
        vd::gl::Context::DepthMask(false);
        vd::gl::Context::CounterClockwiseFacing();
    }

    void SkyRenderer::Finish() {
        vd::gl::Context::Reset();
    }
}
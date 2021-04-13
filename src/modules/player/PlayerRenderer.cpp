#include "PlayerRenderer.hpp"

namespace mod::player {
    PlayerRenderer::PlayerRenderer(PlayerPtr playerPtr, vd::component::IEntityShaderPtr shaderPtr)
        : IRenderer("PlayerRenderer")
        , m_pShader(std::move(shaderPtr))
        , m_pPlayer(std::move(playerPtr))
    {
    }

    void PlayerRenderer::Link() {
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pShadowShader = vd::ObjectOfType<mod::shadow::ShadowShader>::Find();
    }

    void PlayerRenderer::OnInit() {
        m_pPlayer->Init();

        m_pShader->Init();
        m_pShader->Bind();
        m_pShader->InitUniforms(m_pPlayer);
        m_pShader->Unbind();
    }

    void PlayerRenderer::OnUpdate() {
        m_pPlayer->Update();
    }

    void PlayerRenderer::OnRender(const params_t& params) {
        using vd::collision::Detector;

        const vd::component::IEntityShaderPtr& pShader = (params.at("RenderingPass") == "Shadow") ? m_pShadowShader : m_pShader;

        pShader->Bind();

        vd::gl::BufferPtrVec& buffers = m_pPlayer->Buffers();

        const float distanceToCamera = glm::length(m_pCamera->Position() - m_pPlayer->WorldTransform().Translation());
        const auto levelOfDetail = m_pPlayer->LevelOfDetailAtDistance(distanceToCamera);

        auto& meshes = m_pPlayer->Meshes(levelOfDetail);
        auto& boundingBoxes = m_pPlayer->BoundingBoxes(levelOfDetail);
        auto& bufferIndices = m_pPlayer->BufferIndices(levelOfDetail);

        for (size_t meshIndex = 0; meshIndex < meshes.size(); ++meshIndex) {
            pShader->UpdateUniforms(m_pPlayer, levelOfDetail, meshIndex);
            const int count = meshes[meshIndex]->Indices().size();
            buffers[ bufferIndices[meshIndex] ]->DrawElements(vd::gl::eTriangles, count, vd::gl::eUnsignedInt);
        }

        pShader->Unbind();
    }

    void PlayerRenderer::OnCleanUp() {
        m_pPlayer->CleanUp();
        m_pShader->CleanUp();
    }

    bool PlayerRenderer::Precondition(const params_t& params) {
        if (m_pShader == nullptr || m_pPlayer == nullptr) {
            return false;
        }

        const auto& renderingPass = params.at("RenderingPass");
        return (renderingPass == "Shadow" || renderingPass == "Main");
    }

    void PlayerRenderer::Prepare() {
        vd::gl::Context::CounterClockwiseFacing();
    }

    void PlayerRenderer::Finish() {
        vd::gl::Context::Reset();
    }
}
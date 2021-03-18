#include "PlayerRenderer.hpp"

namespace mod::player {
    PlayerRenderer::PlayerRenderer(PlayerPtr playerPtr,
                                   vd::component::IEntityShaderPtr shaderPtr,
                                   vd::Consumer beforeExecution,
                                   vd::Consumer afterExecution)
        : IRenderer(std::move(shaderPtr), std::move(beforeExecution), std::move(afterExecution))
        , m_pPlayer(std::move(playerPtr))
    {
    }

    void PlayerRenderer::Link() {
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pShadowShader = vd::ObjectOfType<mod::shadow::ShadowShader>::Find();
    }

    void PlayerRenderer::Init() {
        m_pPlayer->Init();

        m_pShader->Init();
        m_pShader->Bind();
        m_pShader->InitUniforms(m_pPlayer);
        m_pShader->Unbind();
    }

    void PlayerRenderer::Update() {
        m_pPlayer->Update();
    }

    void PlayerRenderer::Render(const params_t& params) {
        if (!IsReady()) {
            vd::Logger::warn("PlayerRenderer was not ready to render");
            return;
        }

        using vd::collision::Detector;

        const auto& renderingPass = params.at("RenderingPass");
        if (renderingPass == "Shadow" || renderingPass == "Main") {
            Prepare();

            const vd::component::IEntityShaderPtr& pShader = (renderingPass == "Shadow") ? m_pShadowShader : m_pShader;

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

            Finish();
        }
    }

    void PlayerRenderer::CleanUp() {
        m_pPlayer->CleanUp();
        m_pShader->CleanUp();
    }

    bool PlayerRenderer::IsReady() {
        return IRenderer::IsReady() && m_pPlayer != nullptr;
    }

}
#include "PlayerRenderer.hpp"

namespace mod::player {
    PlayerRenderer::PlayerRenderer(PlayerPtr playerPtr,
                                   vd::gl::EntityShaderPtr shaderPtr,
                                   vd::Consumer beforeExecution,
                                   vd::Consumer afterExecution)
        : IRenderer(std::move(shaderPtr), std::move(beforeExecution), std::move(afterExecution))
        , m_pPlayer(std::move(playerPtr))
    {
    }

    PlayerRenderer::~PlayerRenderer() = default;

    void PlayerRenderer::Link() {
        m_pShadowShader = vd::ObjectOfType<mod::shadow::ShadowShader>::Find();
    }

    void PlayerRenderer::Init() {
        m_pPlayer->Init();

        m_pShader->Bind();
        m_pShader->InitUniforms(m_pPlayer);
    }

    void PlayerRenderer::Update() {
        m_pPlayer->Update();
    }

    void PlayerRenderer::Render(const params_t& params) {
        if (!IsReady()) {
            vd::Logger::warn("PlayerRenderer was not ready to render");
            return;
        }

        const auto& renderingPass = params.at("RenderingPass");
        if (renderingPass == "Reflection" || renderingPass == "Refraction") {
            return;
        }

        Prepare();

        const vd::gl::EntityShaderPtr& shaderPtr = (renderingPass == "Shadow") ? m_pShadowShader : m_pShader;

        shaderPtr->Bind();

        vd::gl::BufferPtrVec& buffers = m_pPlayer->Buffers();
        for (size_t meshIndex = 0; meshIndex < buffers.size(); ++meshIndex) {
            shaderPtr->UpdateUniforms(m_pPlayer, meshIndex);
            buffers[meshIndex]->Render();
        }

        Finish();
    }

    void PlayerRenderer::CleanUp() {
        m_pPlayer->CleanUp();
    }

    bool PlayerRenderer::IsReady() {
        return IRenderer::IsReady() && m_pPlayer != nullptr;
    }

}
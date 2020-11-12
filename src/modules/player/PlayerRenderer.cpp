#include "PlayerRenderer.hpp"

namespace mod::player {
    PlayerRenderer::PlayerRenderer(PlayerPtr playerPtr,
                                   vd::shader::ShaderPtr shaderPtr,
                                   vd::Consumer beforeExecution,
                                   vd::Consumer afterExecution)
        : IRenderer(std::move(shaderPtr), std::move(beforeExecution), std::move(afterExecution))
        , m_PlayerPtr(std::move(playerPtr))
    {
    }

    PlayerRenderer::~PlayerRenderer() = default;

    void PlayerRenderer::Init() {
        m_PlayerPtr->Init();
    }

    void PlayerRenderer::Update() {
        m_PlayerPtr->Update();
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

        vd::shader::ShaderPtr shaderPtr = m_ShaderPtr;
        if (renderingPass == "Shadow") {
            shaderPtr = vd::ObjectOfType<vd::shadow::ShadowShader>::Find();
        }

        shaderPtr->bind();

        vd::buffer::BufferPtrVec& buffers = m_PlayerPtr->Buffers();
        for (size_t meshIndex = 0; meshIndex < buffers.size(); ++meshIndex) {
            shaderPtr->updateUniforms(m_PlayerPtr, meshIndex);
            buffers[meshIndex]->Render();
        }

        Finish();
    }

    void PlayerRenderer::CleanUp() {
        m_PlayerPtr->CleanUp();
    }

    bool PlayerRenderer::IsReady() {
        return IRenderer::IsReady() && m_PlayerPtr != nullptr;
    }
}
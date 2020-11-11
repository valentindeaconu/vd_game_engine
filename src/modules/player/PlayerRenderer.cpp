#include "PlayerRenderer.hpp"

namespace mod::player {
    PlayerRenderer::PlayerRenderer()
        : Renderer()
        , m_PlayerPtr(nullptr)
    {
    }

    PlayerRenderer::~PlayerRenderer() = default;

    void PlayerRenderer::Init() {
        m_PlayerPtr->Init();
    }

    void PlayerRenderer::Update() {
        m_PlayerPtr->Update();
    }

    void PlayerRenderer::Render(const vd::kernel::RenderingPass& renderingPass) {
        if (IsReady() && renderingPass != vd::kernel::eReflection && renderingPass != vd::kernel::eRefraction) {
            if (m_ConfigPtr != nullptr) {
                m_ConfigPtr->enable();
            }

            auto _shaderPtr = renderingPass == vd::kernel::RenderingPass::eShadow ? this->GetShadowShader() : m_ShaderPtr;

            _shaderPtr->bind();
            vd::buffer::BufferPtrVec& buffers = m_PlayerPtr->GetBuffers();
            for (size_t meshIndex = 0; meshIndex < buffers.size(); ++meshIndex) {
                _shaderPtr->updateUniforms(m_PlayerPtr, meshIndex);
                buffers[meshIndex]->Render();
            }

            if (m_ConfigPtr != nullptr) {
                m_ConfigPtr->disable();
            }
        }
    }

    void PlayerRenderer::CleanUp() {
        m_PlayerPtr->CleanUp();
    }

    PlayerPtr& PlayerRenderer::GetPlayer() {
        return m_PlayerPtr;
    }

    const PlayerPtr& PlayerRenderer::GetPlayer() const {
        return m_PlayerPtr;
    }

    void PlayerRenderer::SetPlayer(const PlayerPtr& playerPtr) {
        this->m_PlayerPtr = playerPtr;
    }

    bool PlayerRenderer::IsReady() {
        return Renderer::IsReady() && m_PlayerPtr != nullptr;
    }
}
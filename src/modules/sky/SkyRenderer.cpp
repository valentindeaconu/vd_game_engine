#include "SkyRenderer.hpp"

namespace mod::sky {
    SkyRenderer::SkyRenderer(SkyPtr skyPtr,
                             vd::shader::ShaderPtr shaderPtr,
                             vd::Consumer beforeExecution,
                             vd::Consumer afterExecution)
        : IRenderer(std::move(shaderPtr), std::move(beforeExecution), std::move(afterExecution))
        , m_SkyPtr(std::move(skyPtr))
    {
    }

    SkyRenderer::~SkyRenderer() = default;

    void SkyRenderer::Init() {
        m_SkyPtr->Init();
    }

    void SkyRenderer::Update() {
        m_SkyPtr->Update();
    }

    void SkyRenderer::Render(const params_t& params) {
        if (!IsReady()) {
            vd::Logger::warn("SkyRenderer was not ready to render");
            return;
        }

        const auto& renderingPass = params.at("RenderingPass");

        Prepare();

        vd::shader::ShaderPtr shaderPtr = m_ShaderPtr;
        if (renderingPass == "Shadow") {
            shaderPtr = vd::ObjectOfType<mod::shadow::ShadowShader>::Find();
        }

        shaderPtr->bind();

        vd::gl::BufferPtrVec& buffers = m_SkyPtr->Buffers();
        for (size_t meshIndex = 0; meshIndex < buffers.size(); ++meshIndex) {
            shaderPtr->updateUniforms(m_SkyPtr, meshIndex);
            buffers[meshIndex]->Render();
        }

        Finish();
    }

    void SkyRenderer::CleanUp() {
        m_SkyPtr->CleanUp();
    }

    bool SkyRenderer::IsReady() {
        return IRenderer::IsReady() && m_SkyPtr != nullptr;
    }
}
#include "Renderer.hpp"

namespace vd::component {
    Renderer::Renderer()
        : m_ConfigPtr(nullptr)
        , m_ShaderPtr(nullptr)
    {
    }

    Renderer::~Renderer() = default;

    config::MetaConfigPtr& Renderer::GetRenderConfig() {
        return m_ConfigPtr;
    }

    const config::MetaConfigPtr& Renderer::GetRenderConfig() const {
        return m_ConfigPtr;
    }

    void Renderer::SetRenderConfig(const config::MetaConfigPtr& renderConfigPtr) {
        this->m_ConfigPtr = renderConfigPtr;
    }

    vd::shader::ShaderPtr& Renderer::GetShader() {
        return m_ShaderPtr;
    }

    const vd::shader::ShaderPtr& Renderer::GetShader() const {
        return m_ShaderPtr;
    }

    void Renderer::SetShader(const vd::shader::ShaderPtr& shaderPtr) {
        this->m_ShaderPtr = shaderPtr;
    }

    bool Renderer::IsReady() {
        return m_ShaderPtr != nullptr;
    }

    shadow::ShadowShaderPtr& Renderer::GetShadowShader() const {
        return shadow::GetShadowShader();
    }
}
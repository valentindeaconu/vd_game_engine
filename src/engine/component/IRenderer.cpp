#include "IRenderer.hpp"

namespace vd::component {
    IRenderer::IRenderer(shader::ShaderPtr shaderPtr,
                         vd::Consumer beforeExecution,
                         vd::Consumer afterExecution)
        : m_ShaderPtr(std::move(shaderPtr))
        , m_BeforeExecution(std::move(beforeExecution))
        , m_AfterExecution(std::move(afterExecution))
    {
    }

    IRenderer::~IRenderer() = default;

    void IRenderer::Prepare() {
        m_BeforeExecution();
    }

    void IRenderer::Finish() {
        m_AfterExecution();
    }

    vd::shader::ShaderPtr& IRenderer::Shader() {
        return m_ShaderPtr;
    }

    bool IRenderer::IsReady() {
        return m_ShaderPtr != nullptr;
    }
}
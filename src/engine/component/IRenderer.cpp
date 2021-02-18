#include "IRenderer.hpp"

namespace vd::component {
    IRenderer::IRenderer(IEntityShaderPtr shaderPtr,
                         vd::Consumer beforeExecution,
                         vd::Consumer afterExecution)
        : m_pShader(std::move(shaderPtr))
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

    IEntityShaderPtr& IRenderer::Shader() {
        return m_pShader;
    }

    bool IRenderer::IsReady() {
        return m_pShader != nullptr;
    }
}
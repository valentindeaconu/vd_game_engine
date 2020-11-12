//
// Created by Vali on 9/11/2020.
//

#include "WaterRenderer.hpp"

namespace mod::water {

    WaterRenderer::WaterRenderer(WaterPtr waterPtr,
                                 vd::shader::ShaderPtr shaderPtr,
                                 vd::Consumer beforeExecution,
                                 vd::Consumer afterExecution)
        : IRenderer(std::move(shaderPtr), std::move(beforeExecution), std::move(afterExecution))
        , m_WaterPtr(std::move(waterPtr))
    {
    }

    WaterRenderer::~WaterRenderer() = default;

    void WaterRenderer::Init() {
        m_WaterPtr->Init();
    }

    void WaterRenderer::Update() {
        m_WaterPtr->Update();
    }

    void WaterRenderer::Render(const params_t& params) {
        if (!IsReady()) {
            vd::Logger::warn("WaterRenderer was not ready to render");
            return;
        }

        const auto& renderingPass = params.at("RenderingPass");
        if (renderingPass != "Main") {
            return;
        }

        Prepare();

        m_ShaderPtr->bind();

        m_ShaderPtr->updateUniforms(m_WaterPtr, 0);

        m_WaterPtr->Buffers()[0]->Render();

        Finish();
    }

    void WaterRenderer::CleanUp() {
        m_WaterPtr->CleanUp();
    }

    WaterPtr &WaterRenderer::GetWater() {
        return m_WaterPtr;
    }

    const WaterPtr &WaterRenderer::GetWater() const {
        return m_WaterPtr;
    }

    void WaterRenderer::SetWater(const WaterPtr &waterPtr) {
        this->m_WaterPtr = waterPtr;
    }

    bool WaterRenderer::IsReady() {
        return IRenderer::IsReady() && m_WaterPtr != nullptr;
    }
}
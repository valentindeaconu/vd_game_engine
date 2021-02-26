//
// Created by Vali on 9/11/2020.
//

#include "WaterRenderer.hpp"

namespace mod::water {

    WaterRenderer::WaterRenderer(WaterPtr waterPtr,
                                 vd::component::IEntityShaderPtr shaderPtr,
                                 vd::Consumer beforeExecution,
                                 vd::Consumer afterExecution)
        : IRenderer(std::move(shaderPtr), std::move(beforeExecution), std::move(afterExecution))
        , m_pWater(std::move(waterPtr))
    {
    }

    WaterRenderer::~WaterRenderer() = default;

    void WaterRenderer::Init() {
        m_pWater->Init();

        m_pShader->Bind();
        m_pShader->InitUniforms(m_pWater);
    }

    void WaterRenderer::Update() {
        m_pWater->Update();
    }

    void WaterRenderer::Render(const params_t& params) {
        if (!IsReady()) {
            vd::Logger::warn("WaterRenderer was not ready to render");
            return;
        }

        const auto& renderingPass = params.at("RenderingPass");
        if (renderingPass == "Main") {
            Prepare();

            m_pShader->Bind();

            m_pShader->UpdateUniforms(m_pWater, 0);

            m_pWater->Buffers()[0]->DrawElements(vd::gl::eTriangles, 6, vd::gl::eUnsignedInt);

            Finish();
        }
    }

    void WaterRenderer::CleanUp() {
        m_pWater->CleanUp();
    }

    WaterPtr& WaterRenderer::Water() {
        return m_pWater;
    }

    bool WaterRenderer::IsReady() {
        return IRenderer::IsReady() && m_pWater != nullptr;
    }
}
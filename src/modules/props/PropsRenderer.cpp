#include "PropsRenderer.hpp"

namespace mod::props {
    PropsRenderer::PropsRenderer(vd::component::IEntityShaderPtr shaderPtr,
                                 vd::Consumer beforeExecution,
                                 vd::Consumer afterExecution)
        : IRenderer(std::move(shaderPtr), std::move(beforeExecution), std::move(afterExecution))
        , m_pPropGenerator(std::make_shared<PropGenerator>(6000)) // TODO: Read prop count from a properties file
    {
    }

    PropsRenderer::~PropsRenderer() = default;

    void PropsRenderer::Link() {
        m_pShadowShader = vd::ObjectOfType<mod::shadow::ShadowShader>::Find();
        m_pFrustumCullingManager = vd::ObjectOfType<vd::culling::FrustumCullingManager>::Find();
    }

    void PropsRenderer::Init() {
        m_pPropGenerator->GenerateLocations();

        m_pShader->Bind();
        m_pShader->InitUniforms(nullptr);
    }

    void PropsRenderer::Update() {

    }

    void PropsRenderer::Render(const params_t& params) {
        if (!IsReady()) {
            vd::Logger::warn("PropsRenderer was not ready to render");
            return;
        }

        using vd::collision::Detector;

        const auto& renderingPass = params.at("RenderingPass");

        Prepare();

        vd::component::IEntityShaderPtr pShader = (renderingPass != "Shadow") ? m_pShader : m_pShadowShader;

        pShader->Bind();

        for (const auto& placement : m_pPropGenerator->Placements()) {
            const PropPtr& pProp = placement.Prop;

            pProp->WorldTransform().Translation() = placement.Location;

            if (Detector::IsAnyTransformedBounds3InsideFrustum(pProp->BoundingBoxes(),
                                                               pProp->WorldTransform(),
                                                               m_pFrustumCullingManager->Frustum())) {
                for (int mId = 0; mId < pProp->Buffers().size(); ++mId) {
                    pShader->UpdateUniforms(pProp, mId);

                    const int count = pProp->Meshes()[mId]->Indices().size();
                    pProp->Buffers()[mId]->DrawElements(vd::gl::eTriangles, count, vd::gl::eUnsignedInt);
                }
            }
        }

        Finish();
    }

    void PropsRenderer::CleanUp() {

    }

    bool PropsRenderer::IsReady() {
        return IRenderer::IsReady() && m_pPropGenerator != nullptr;
    }

}
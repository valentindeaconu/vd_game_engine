#include "PropsRenderer.hpp"

namespace mod::props {
    PropsRenderer::PropsRenderer(vd::component::IEntityShaderPtr shaderPtr,
                                 vd::Consumer beforeExecution,
                                 vd::Consumer afterExecution)
        : IRenderer(std::move(shaderPtr), std::move(beforeExecution), std::move(afterExecution))
        , m_pPropGenerator(std::make_shared<PropGenerator>(4500)) // TODO: Read prop count from a properties file
    {
    }

    PropsRenderer::~PropsRenderer() = default;

    void PropsRenderer::Link() {
        // TODO: Remove dependency (used only for debugging)
        m_pEventHandler = vd::ObjectOfType<vd::event::EventHandler>::Find();

        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pFrustumCullingManager = vd::ObjectOfType<vd::culling::FrustumCullingManager>::Find();
        m_pShadowShader = vd::ObjectOfType<mod::shadow::ShadowShader>::Find();
    }

    void PropsRenderer::Init() {
        m_pPropGenerator->GenerateLocations();

        m_pShader->Bind();
        m_pShader->InitUniforms(nullptr);
    }

    void PropsRenderer::Update() {
        float cameraYaw = m_pCamera->Yaw() + 180.0f;
        cameraYaw = (cameraYaw > 360.0f) ? cameraYaw - 360.0f : cameraYaw;

        for (const auto& placement : m_pPropGenerator->Placements()) {
            const PropPtr& pProp = placement.Prop;

            const glm::vec3 toProp = placement.Location - m_pCamera->Position();
            const float distanceToCamera = glm::length(toProp);
            const auto levelOfDetail = pProp->LevelOfDetailAtDistance(distanceToCamera);

            if (pProp->BillboardAtLevel(levelOfDetail)) {
                pProp->WorldTransform().YAxisRotationAngle() = cameraYaw;
            } else {
                pProp->WorldTransform().YAxisRotationAngle() = 0.0f;
            }
        }
    }

    void PropsRenderer::Render(const params_t& params) {
        if (!IsReady()) {
            vd::Logger::warn("PropsRenderer was not ready to render");
            return;
        }

        using vd::collision::Detector;

        const auto& renderingPass = params.at("RenderingPass");

        if (renderingPass == "Shadow" ||
            renderingPass == "Reflection" ||
            renderingPass == "Refraction" ||
            renderingPass == "Main") {
            Prepare();

            vd::component::IEntityShaderPtr pShader = (renderingPass != "Shadow") ? m_pShader : m_pShadowShader;

            pShader->Bind();

            for (const auto& placement : m_pPropGenerator->Placements()) {
                const PropPtr& pProp = placement.Prop;

                pProp->WorldTransform().Translation() = placement.Location;

                const float distanceToCamera = glm::length(m_pCamera->Position() - placement.Location);
                const auto levelOfDetail = pProp->LevelOfDetailAtDistance(distanceToCamera);

                auto& boundingBoxes = pProp->BoundingBoxes(levelOfDetail);

                if (Detector::IsAnyTransformedBounds3InsideFrustum(boundingBoxes,
                                                                   pProp->WorldTransform(),
                                                                   m_pFrustumCullingManager->Frustum())) {
                    auto& meshes = pProp->Meshes(levelOfDetail);
                    auto& bufferIndices = pProp->BufferIndices(levelOfDetail);
                    auto& buffers = pProp->Buffers();

                    for (int meshIndex = 0; meshIndex < meshes.size(); ++meshIndex) {
                        pShader->UpdateUniforms(pProp, levelOfDetail, meshIndex);

                        const int count = meshes[meshIndex]->Indices().size();

                        buffers[ bufferIndices[meshIndex] ]->DrawElements(vd::gl::eTriangles, count, vd::gl::eUnsignedInt);
                    }
                }
            }

            Finish();
        }
    }

    void PropsRenderer::CleanUp() {

    }

    bool PropsRenderer::IsReady() {
        return IRenderer::IsReady() && m_pPropGenerator != nullptr;
    }

}
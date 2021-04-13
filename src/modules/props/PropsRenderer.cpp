#include "PropsRenderer.hpp"

namespace mod::props {
    PropsRenderer::PropsRenderer(PropsManagerPtr propsManager, vd::component::IEntityShaderPtr shaderPtr)
        : IRenderer("PropsRenderer")
        , m_pShader(std::move(shaderPtr))
        , m_pPropsManager(std::move(propsManager))
    {
    }

    void PropsRenderer::Link() {
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pFrustumCullingManager = vd::ObjectOfType<vd::culling::FrustumCullingManager>::Find();
        m_pShadowShader = vd::ObjectOfType<mod::shadow::ShadowShader>::Find();
    }

    void PropsRenderer::OnInit() {
        m_pPropsManager->Init();

        const auto& placements = m_pPropsManager->Placements();
        m_Units.Total = placements.size();

        m_Units.Transforms.resize(m_Units.Total);
        m_Units.Props.resize(m_Units.Total);
        m_Units.Levels.resize(m_Units.Total);
        m_Units.Culled.resize(m_Units.Total);

        for (int i = 0; i < m_Units.Total; ++i) {
            const auto& placement = placements[i];
            m_Units.Props[i] = placement.Prop;
            m_Units.Levels[i] = 0;
            m_Units.Transforms[i] = placement.Prop->WorldTransform();
            m_Units.Transforms[i].Translation() = placement.Location;
            m_Units.Culled[i] = false;
        }

        m_pShader->Init();
        m_pShader->Bind();
        m_pShader->InitUniforms(nullptr);
        m_pShader->Unbind();
    }

    void PropsRenderer::OnUpdate() {
        using vd::collision::Detector;
        using vd::collision::Relationship;

        float cameraYaw = m_pCamera->Yaw() + 180.0f;
        cameraYaw = (cameraYaw > 360.0f) ? cameraYaw - 360.0f : cameraYaw;

        for (size_t i = 0; i < m_Units.Total; ++i) {
            auto& Prop = m_Units.Props[i];
            auto& Transform = m_Units.Transforms[i];

            const float distanceToCamera = glm::length(m_pCamera->Position() - Transform.Translation());
            const auto levelOfDetail = Prop->LevelOfDetailAtDistance(distanceToCamera);

            m_Units.Levels[i] = levelOfDetail;

            if (Prop->BillboardAtLevel(levelOfDetail)) {
                Transform.YAxisRotationAngle() = cameraYaw;
            } else {
                Transform.YAxisRotationAngle() = 0.0f;
            }

            // Checking against frustum's bounding box
            const auto& boundingBoxes = Prop->BoundingBoxes(levelOfDetail);
            const auto& fBounds =  m_pFrustumCullingManager->FrustumBounds();

            bool found = false;
            for (const auto& bounds : boundingBoxes) {
                if (Detector::Bounds3AgainstBounds3(bounds.WithTransform(Transform), fBounds) != Relationship::eOutside) {
                    found = true;
                    break;
                }
            }
            m_Units.Culled[i] = !found;
        }
    }

    void PropsRenderer::OnRender(const params_t& params) {
        using vd::collision::Detector;

        vd::component::IEntityShaderPtr pShader = (params.at("RenderingPass") != "Shadow") ? m_pShader : m_pShadowShader;

        pShader->Bind();

        for (size_t i = 0; i < m_Units.Total; ++i) {
            if (!m_Units.Culled[i]) {
                auto& Prop = m_Units.Props[i];
                auto& Level = m_Units.Levels[i];
                auto& Transform = m_Units.Transforms[i];

                Prop->WorldTransform() = Transform;

                auto& meshes = Prop->Meshes(Level);
                auto& bufferIndices = Prop->BufferIndices(Level);
                auto& buffers = Prop->Buffers();

                for (int meshIndex = 0; meshIndex < meshes.size(); ++meshIndex) {
                    pShader->UpdateUniforms(Prop, Level, meshIndex);
                    const int count = meshes[meshIndex]->Indices().size();
                    buffers[bufferIndices[meshIndex]]->DrawElements(vd::gl::eTriangles,
                                                                    count,
                                                                    vd::gl::eUnsignedInt);
                }
            }
        }

        pShader->Unbind();
    }

    void PropsRenderer::OnCleanUp() {
        m_pPropsManager->CleanUp();
        m_pShader->CleanUp();
    }

    bool PropsRenderer::Precondition(const params_t& params) {
        if (m_pPropsManager == nullptr || m_pShader == nullptr) {
            return false;
        }

        const auto& renderingPass = params.at("RenderingPass");
        return (renderingPass == "Shadow" ||
                renderingPass == "Reflection" ||
                renderingPass == "Refraction" ||
                renderingPass == "Main");
    }

    void PropsRenderer::Prepare() {
        vd::gl::Context::CounterClockwiseFacing();
    }

    void PropsRenderer::Finish() {
        vd::gl::Context::Reset();
    }
}
#include "StaticObjectRenderer.hpp"

namespace mod::sobj {
    StaticObjectRenderer::StaticObjectRenderer(StaticObjectPlacerPtr staticObjectPlacerPtr,
                                               vd::gl::ShaderPtr shaderPtr,
                                               vd::Consumer beforeExecution,
                                               vd::Consumer afterExecution)
        : IRenderer(std::move(shaderPtr), std::move(beforeExecution), std::move(afterExecution))
        , m_StaticObjectPlacerPtr(staticObjectPlacerPtr)
    {
    }

    StaticObjectRenderer::~StaticObjectRenderer() = default;

    void StaticObjectRenderer::Init() {
        m_FrustumCullingManagerPtr = vd::ObjectOfType<vd::culling::FrustumCullingManager>::Find();

        if (IsReady()) {
            m_StaticObjectPlacerPtr->place();
        }

        m_pShader->Bind();
        m_pShader->InitUniforms(nullptr);
    }

    void StaticObjectRenderer::Update() {

    }

    void StaticObjectRenderer::Render(const params_t& params) {
        if (!IsReady()) {
            vd::Logger::warn("StaticObjectRenderer was not ready to render");
            return;
        }

        using vd::collision::Detector;

        const auto& renderingPass = params.at("RenderingPass");

        Prepare();

        vd::gl::ShaderPtr shaderPtr = m_pShader;
        if (renderingPass == "Shadow") {
            shaderPtr = vd::ObjectOfType<mod::shadow::ShadowShader>::Find();
        }

        shaderPtr->Bind();

        const PlacementInfoVec &placementInfos = m_StaticObjectPlacerPtr->getPlacementInfos();
        for (const auto& placementInfo : placementInfos) {
            StaticObjectPtr staticObjectPtr = placementInfo.objectPtr;
            staticObjectPtr->WorldTransform().Translation() = placementInfo.location;
            staticObjectPtr->Update();

            if (Detector::IsAnyTransformedBounds3InsideFrustum(staticObjectPtr->BoundingBoxes(),
                                                               staticObjectPtr->WorldTransform(),
                                                               m_FrustumCullingManagerPtr->Frustum())) {
                for (size_t meshIndex = 0;
                     meshIndex < staticObjectPtr->Buffers().size();
                     ++meshIndex) {
                    shaderPtr->UpdateUniforms(staticObjectPtr, meshIndex);
                    staticObjectPtr->Buffers()[meshIndex]->Render();
                }
            }
        }

        Finish();
    }

    void StaticObjectRenderer::CleanUp() {
        const auto& biomeAtlas = m_StaticObjectPlacerPtr->getTerrain()->Biomes();

        for (const auto& biome : biomeAtlas) {
            if (!biome->getObjects().empty()) {
                for (const auto& object : biome->getObjects()) {
                    object->CleanUp();
                }
            }
        }
    }

    bool StaticObjectRenderer::IsReady() {
        return IRenderer::IsReady() && m_StaticObjectPlacerPtr != nullptr;
    }
}
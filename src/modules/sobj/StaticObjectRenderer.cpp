#include "StaticObjectRenderer.hpp"

namespace mod::sobj
{
    StaticObjectRenderer::StaticObjectRenderer()
        : Renderer()
        , m_StaticObjectPlacerPtr(nullptr)
    {
    }

    StaticObjectRenderer::~StaticObjectRenderer() = default;

    void StaticObjectRenderer::Init() {
        m_FrustumCullingManagerPtr = vd::ObjectOfType<vd::culling::FrustumCullingManager>::Find();

        if (IsReady()) {
            m_StaticObjectPlacerPtr->place();
        }
    }

    void StaticObjectRenderer::Update() {

    }

    void StaticObjectRenderer::Render(const vd::kernel::RenderingPass &renderingPass) {
        using vd::collision::Detector;

        if (IsReady()) {
            if (m_ConfigPtr != nullptr) {
                m_ConfigPtr->enable();
            }

            auto _shaderPtr = renderingPass == vd::kernel::RenderingPass::eShadow ? this->GetShadowShader() : m_ShaderPtr;

            _shaderPtr->bind();

            const PlacementInfoVec &placementInfos = m_StaticObjectPlacerPtr->getPlacementInfos();
            for (const auto& placementInfo : placementInfos) {
                StaticObjectPtr staticObjectPtr = placementInfo.objectPtr;
                staticObjectPtr->GetWorldTransform().SetTranslation(placementInfo.location);
                staticObjectPtr->Update();

                if (Detector::IsAnyTransformedBounds3InsideFrustum(staticObjectPtr->GetBoundingBoxes(),
                                                                   staticObjectPtr->GetWorldTransform(),
                                                                   m_FrustumCullingManagerPtr->GetFrustum())) {
                    for (size_t meshIndex = 0;
                         meshIndex < staticObjectPtr->GetBuffers().size();
                         ++meshIndex) {
                        _shaderPtr->updateUniforms(staticObjectPtr, meshIndex);
                        staticObjectPtr->GetBuffers()[meshIndex]->Render();
                    }
                }
            }

            if (m_ConfigPtr != nullptr) {
                m_ConfigPtr->disable();
            }
        }
    }

    void StaticObjectRenderer::CleanUp() {
        const auto& biomeAtlas = m_StaticObjectPlacerPtr->getTerrain()->GetBiomes();

        for (const auto& biome : biomeAtlas) {
            if (!biome->getObjects().empty()) {
                for (const auto& object : biome->getObjects()) {
                    object->CleanUp();
                }
            }
        }
    }

    StaticObjectPlacerPtr& StaticObjectRenderer::GetStaticObjectPlacer() {
        return m_StaticObjectPlacerPtr;
    }

    const StaticObjectPlacerPtr& StaticObjectRenderer::GetStaticObjectPlacer() const {
        return m_StaticObjectPlacerPtr;
    }

    void StaticObjectRenderer::SetStaticObjectPlacer(const StaticObjectPlacerPtr& staticObjectPlacerPtr) {
        this->m_StaticObjectPlacerPtr = staticObjectPlacerPtr;
    }

    bool StaticObjectRenderer::IsReady() {
        return Renderer::IsReady() && m_StaticObjectPlacerPtr != nullptr;
    }
}
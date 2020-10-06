#include "StaticObjectRenderer.hpp"

namespace mod::sobj
{
    StaticObjectRenderer::StaticObjectRenderer()
        : Renderer()
        , staticObjectPlacerPtr(nullptr)
    {
    }

    StaticObjectRenderer::~StaticObjectRenderer() = default;

    void StaticObjectRenderer::init()
    {
        if (isReady())
        {
            staticObjectPlacerPtr->place();
        }
    }

    void StaticObjectRenderer::update() {

    }

    void StaticObjectRenderer::render(const vd::kernel::RenderingPass &renderingPass) {
        if (isReady()) {
            if (renderConfigPtr != nullptr) {
                renderConfigPtr->enable();
            }

            auto _shaderPtr = renderingPass == vd::kernel::RenderingPass::eShadow ? this->getShadowShader() : shaderPtr;

            _shaderPtr->bind();

            const PlacementInfoMat &placementInfosForBiomes = staticObjectPlacerPtr->getPlacementInfosForBiomes();
            const auto& biomeAtlas =
                    staticObjectPlacerPtr->getTerrain()->GetTerrainConfig()->getBiomes();

            // TODO: Adapt to the new terrain
            /*for (size_t biomeIndex = 0; biomeIndex < biomeAtlas.size(); ++biomeIndex) {
                const auto& biome = biomeAtlas[biomeIndex];
                const PlacementInfoVec &placementInfos = placementInfosForBiomes[biomeIndex];

                if (!placementInfos.empty()) {
                    for (const auto& placementInfo : placementInfos) {
                        StaticObjectPtr staticObjectPtr = biome.objects[placementInfo.objectIndex];
                        staticObjectPtr->getWorldTransform().setTranslation(placementInfo.location);
                        staticObjectPtr->update();

                        if (staticObjectPtr->shouldBeRendered()) {
                            for (size_t meshIndex = 0;
                                 meshIndex < staticObjectPtr->getBuffers().size();
                                 ++meshIndex) {
                                _shaderPtr->updateUniforms(staticObjectPtr, meshIndex);
                                staticObjectPtr->getBuffers()[meshIndex]->render();
                            }
                        }
                    }
                }
            }*/

            if (renderConfigPtr != nullptr) {
                renderConfigPtr->disable();
            }
        }
    }

    void StaticObjectRenderer::cleanUp() {
        const auto& biomeAtlas = staticObjectPlacerPtr->getTerrain()->GetTerrainConfig()->getBiomes();

        // TODO: Adapt to the new terrain
        /*for (const auto& biome : biomeAtlas) {
            if (!biome.objects.empty()) {
                for (const auto& object : biome.objects) {
                    object->cleanUp();
                }
            }
        }*/
    }

    StaticObjectPlacerPtr& StaticObjectRenderer::getStaticObjectPlacer() {
        return staticObjectPlacerPtr;
    }

    const StaticObjectPlacerPtr& StaticObjectRenderer::getStaticObjectPlacer() const {
        return staticObjectPlacerPtr;
    }

    void StaticObjectRenderer::setStaticObjectPlacer(const StaticObjectPlacerPtr& staticObjectPlacerPtr) {
        this->staticObjectPlacerPtr = staticObjectPlacerPtr;
    }

    bool StaticObjectRenderer::isReady() {
        return Renderer::isReady() && staticObjectPlacerPtr != nullptr;
    }
}
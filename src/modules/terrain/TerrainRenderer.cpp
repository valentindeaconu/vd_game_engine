#include "TerrainRenderer.hpp"

namespace mod::terrain
{
    TerrainRenderer::TerrainRenderer()
        : Renderer()
        , terrainPtr(nullptr)
    {
    }

    TerrainRenderer::~TerrainRenderer() = default;

    void TerrainRenderer::init() {
        terrainPtr->init();
    }

    void TerrainRenderer::update() {
        terrainPtr->update();
    }

    void TerrainRenderer::render(const vd::kernel::RenderingPass& renderingPass) {
        if (isReady()) {
            if (renderConfigPtr != nullptr) {
                renderConfigPtr->enable();
            }

            auto _shaderPtr = renderingPass == vd::kernel::RenderingPass::eShadow ? this->getShadowShader() : shaderPtr;

            _shaderPtr->bind();
            _shaderPtr->updateUniforms(terrainPtr, 0);
            terrainPtr->getMeshBuffers()[0]->render();

            if (renderConfigPtr != nullptr) {
                renderConfigPtr->disable();
            }
        }
    }

    void TerrainRenderer::cleanUp()
    {
        terrainPtr->cleanUp();
    }

    TerrainPtr& TerrainRenderer::getTerrain()
    {
        return terrainPtr;
    }

    const TerrainPtr& TerrainRenderer::getTerrain() const
    {
        return terrainPtr;
    }

    void TerrainRenderer::setTerrain(const TerrainPtr& terrainPtr)
    {
        this->terrainPtr = terrainPtr;
    }

    bool TerrainRenderer::isReady()
    {
        return Renderer::isReady() && terrainPtr != nullptr;
    }
}
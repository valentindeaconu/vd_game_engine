//
// Created by Vali on 9/21/2020.
//

#include "TerrainRenderer.hpp"

namespace mod::terrain {

    TerrainRenderer::TerrainRenderer()
        : vd::component::Renderer()
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
        if (isReady() && renderingPass == vd::kernel::eMain) {
            if (renderConfigPtr != nullptr) {
                renderConfigPtr->enable();
            }

            const auto& rootNodes = terrainPtr->GetRootNodes();
            const auto& terrainConfigPtr = terrainPtr->GetTerrainConfig();

            for (const auto& rootNode : rootNodes) {
                renderNode(rootNode, terrainConfigPtr);
            }

            if (renderConfigPtr != nullptr) {
                renderConfigPtr->disable();
            }
        }
    }

    void TerrainRenderer::renderNode(const TerrainNodePtr& nodePtr, const TerrainConfigPtr& terrainConfigPtr) {
        if (nodePtr != nullptr) {
            if (nodePtr->IsLeaf()) {
                shaderPtr->bind();

                shaderPtr->setUniform("localModel", nodePtr->GetTransform().get());
                shaderPtr->setUniform("worldModel", terrainConfigPtr->getTransform().get());

                const auto& patchHeights = nodePtr->GetPatchHeights();
                for (int i = 0; i < 16; ++i) {
                    shaderPtr->setUniformf("patchHeights[" + std::to_string(i) + "]", patchHeights[i]);
                }

                shaderPtr->updateUniforms(terrainPtr, 0);

                vd::buffer::BufferPtrVec& buffers = terrainPtr->getBuffers();
                buffers[0]->render();
            } else {
                const auto& children = nodePtr->GetNodes();
                for (const auto& child : children) {
                    renderNode(child, terrainConfigPtr);
                }
            }
        }
    }

    void TerrainRenderer::cleanUp() {
        terrainPtr->cleanUp();
    }

    TerrainPtr& TerrainRenderer::getTerrain() {
        return terrainPtr;
    }

    const TerrainPtr& TerrainRenderer::getTerrain() const {
        return terrainPtr;
    }

    void TerrainRenderer::setTerrain(const TerrainPtr& terrainPtr) {
        this->terrainPtr = terrainPtr;
    }

    bool TerrainRenderer::isReady() {
        return Renderer::isReady() && terrainPtr != nullptr;
    }
}
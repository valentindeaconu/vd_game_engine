//
// Created by Vali on 9/21/2020.
//

#include "TerrainRenderer.hpp"

namespace vd::terrain {

    TerrainRenderer::TerrainRenderer()
        : component::Renderer()
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

    void TerrainRenderer::render(const kernel::RenderingPass& renderingPass) {
        if (isReady() && renderingPass == kernel::eMain) {
            if (renderConfigPtr != nullptr) {
                renderConfigPtr->enable();
            }

            const auto& terrainQuadtreePtr = terrainPtr->getTerrainQuadtree();
            const auto& rootNodes = terrainQuadtreePtr->getRootNodes();

            for (const auto& nodePtr : rootNodes) {
                renderNode(nodePtr);
            }

            if (renderConfigPtr != nullptr) {
                renderConfigPtr->disable();
            }
        }
    }

    void TerrainRenderer::renderNode(const TerrainNodePtr& nodePtr) {
        if (nodePtr != nullptr) {
            if (nodePtr->isLeaf()) {
                shaderPtr->bind();
                shaderPtr->updateUniforms(nodePtr, 0);

                const auto& terrainQuadtreePtr = terrainPtr->getTerrainQuadtree();
                vd::buffer::BufferPtrVec& buffers = terrainQuadtreePtr->getBuffers();
                buffers[0]->render();
            } else {
                for (const auto& subNodePtr : nodePtr->getNodes()) {
                    renderNode(subNodePtr);
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
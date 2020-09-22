//
// Created by Vali on 9/21/2020.
//

#include "Terrain.hpp"

namespace vd::terrain {

    Terrain::Terrain(const EnginePtr& enginePtr, const std::string& configFilePath)
        : object::Entity(enginePtr)
    {
        configPtr = std::make_shared<TerrainConfig>(configFilePath);
        terrainQuadtreePtr = std::make_shared<TerrainQuadtree>(enginePtr, configPtr);
    }

    Terrain::~Terrain() = default;

    void Terrain::init() {
        configPtr->parse();

        terrainQuadtreePtr->init();

        Entity::init();
    }

    void Terrain::update() {
        auto& cameraPtr = getParentEngine()->getCamera();
        if (cameraPtr->isCameraMoved() || cameraPtr->isCameraRotated()) {
            terrainQuadtreePtr->update();
        }
    }

    void Terrain::cleanUp() {
        terrainQuadtreePtr->cleanUp();

        Entity::cleanUp();
    }

    const TerrainConfigPtr& Terrain::getTerrainConfig() const {
        return configPtr;
    }

    const TerrainQuadtreePtr& Terrain::getTerrainQuadtree() const {
        return terrainQuadtreePtr;
    }
}
//
// Created by Vali on 9/21/2020.
//

#include "TerrainShader.hpp"

namespace mod::terrain {

    TerrainShader::TerrainShader()
        : vd::shader::Shader()
        , kBiomeCount(5)
    {
        loadAndAddShader("./resources/shaders/terrain/terrain_VS.glsl", vd::shader::eVertexShader);
        loadAndAddShader("./resources/shaders/terrain/terrain_TC.glsl", vd::shader::eTessellationControlShader);
        loadAndAddShader("./resources/shaders/terrain/terrain_TE.glsl", vd::shader::eTessellationEvaluationShader);
        loadAndAddShader("./resources/shaders/terrain/terrain_GS.glsl", vd::shader::eGeometryShader);
        loadAndAddShader("./resources/shaders/terrain/terrain_FS.glsl", vd::shader::eFragmentShader);

        compileShader();

        addUniform("localModel");
        addUniform("worldModel");
        addUniform("view");
        addUniform("projection");

        addUniform("cameraPosition");

        //addUniform("scaleY");
        addUniform("tessellationFactor");
        addUniform("tessellationSlope");
        addUniform("tessellationShift");
        addUniform("tbnRange");

        addUniform("heightMap");
        addUniform("normalMap");
        addUniform("splatMap");

        for (int i = 0; i < 16; ++i) {
            addUniform("patchHeights[" + std::to_string(i) + "]");
        }

        for (int i = 0; i < kBiomeCount; ++i) {
            addUniform("materials[" + std::to_string(i) + "].diffuseMap");
            addUniform("materials[" + std::to_string(i) + "].normalMap");
            addUniform("materials[" + std::to_string(i) + "].horizontalScaling");
        }

        for (int i = 0; i < kBiomeCount; ++i) {
            addUniform("materials[" + std::to_string(i) + "].diffuseMap");
            addUniform("materials[" + std::to_string(i) + "].normalMap");
            addUniform("materials[" + std::to_string(i) + "].horizontalScaling");
        }
    }

    TerrainShader::~TerrainShader() = default;

    void TerrainShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) {
        auto& enginePtr = entityPtr->getParentEngine();
        setUniform("view", enginePtr->getCamera()->getViewMatrix());
        setUniform("projection", enginePtr->getWindow()->getProjectionMatrix());
        setUniform("cameraPosition", enginePtr->getCamera()->getPosition());

        const auto& terrainPtr = std::dynamic_pointer_cast<Terrain>(entityPtr);
        const auto& configPtr = terrainPtr->GetTerrainConfig();

        //setUniformi("scaleY", configPtr->getScaleY());
        setUniformi("tessellationFactor", configPtr->getTessellationFactor());
        setUniformf("tessellationSlope", configPtr->getTessellationSlope());
        setUniformf("tessellationShift", configPtr->getTessellationShift());
        setUniformi("tbnRange", configPtr->getTbnRange());

        vd::model::activeTexture(0);
        configPtr->getHeightMap()->bind();
        setUniformi("heightMap", 0);

        vd::model::activeTexture(1);
        configPtr->getNormalMap()->bind();
        setUniformi("normalMap", 1);

        vd::model::activeTexture(2);
        configPtr->getSplatMap()->bind();
        setUniformi("splatMap", 2);

        int textureUnit = 3;
        for (int i = 0; i < kBiomeCount; ++i) {
            vd::model::Material& material = configPtr->getBiomes()[i]->getMaterial();

            vd::model::activeTexture(textureUnit);
            material.diffuseMap->bind();
            setUniformi("materials[" + std::to_string(i) + "].diffuseMap", textureUnit);
            ++textureUnit;

            vd::model::activeTexture(textureUnit);
            material.normalMap->bind();
            setUniformi("materials[" + std::to_string(i) + "].normalMap", textureUnit);
            ++textureUnit;

            setUniformf("materials[" + std::to_string(i) + "].horizontalScaling", material.horizontalScale);
        }
    }

}
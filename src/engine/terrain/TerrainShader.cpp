//
// Created by Vali on 9/21/2020.
//

#include "TerrainShader.hpp"

namespace vd::terrain {

    TerrainShader::TerrainShader()
        : shader::Shader()
        , kBiomeCount(5)
    {
        loadAndAddShader("./resources/shaders/terrain2/terrain_VS.glsl", vd::shader::eVertexShader);
        loadAndAddShader("./resources/shaders/terrain2/terrain_TC.glsl", vd::shader::eTessellationControlShader);
        loadAndAddShader("./resources/shaders/terrain2/terrain_TE.glsl", vd::shader::eTessellationEvaluationShader);
        loadAndAddShader("./resources/shaders/terrain2/terrain_GS.glsl", vd::shader::eGeometryShader);
        loadAndAddShader("./resources/shaders/terrain2/terrain_FS.glsl", vd::shader::eFragmentShader);

        compileShader();

        addUniform("localModel");
        addUniform("worldModel");
        addUniform("view");
        addUniform("projection");

        addUniform("cameraPosition");

        addUniform("scaleY");

        addUniform("lod");
        addUniform("index");
        addUniform("gap");
        addUniform("position");

        addUniform("tessellationFactor");
        addUniform("tessellationSlope");
        addUniform("tessellationShift");
        addUniform("tbnRange");

        addUniform("heightMap");
        addUniform("normalMap");
        addUniform("splatMap");

        addUniform("lodMorphArea");

        for (int i = 0; i < kBiomeCount; ++i) {
            addUniform("materials[" + std::to_string(i) + "].diffuseMap");
            addUniform("materials[" + std::to_string(i) + "].normalMap");
            addUniform("materials[" + std::to_string(i) + "].displaceMap");
            addUniform("materials[" + std::to_string(i) + "].heightScaling");
            addUniform("materials[" + std::to_string(i) + "].horizontalScaling");
        }
    }

    TerrainShader::~TerrainShader() = default;

    void TerrainShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) {
        setUniform("localModel", entityPtr->getLocalTransform().get());
        setUniform("worldModel", entityPtr->getWorldTransform().get());

        auto& enginePtr = entityPtr->getParentEngine();
        setUniform("view", enginePtr->getCamera()->getViewMatrix());
        setUniform("projection", enginePtr->getWindow()->getProjectionMatrix());

        setUniform("cameraPosition", enginePtr->getCamera()->getPosition());

        const auto& nodePtr = std::dynamic_pointer_cast<TerrainNode>(entityPtr);
        setUniformi("lod", nodePtr->getLod());
        setUniform("index", nodePtr->getIndex());
        setUniformf("gap", nodePtr->getGap());
        setUniform("position", nodePtr->getPosition());

        const auto& configPtr = nodePtr->getConfig();
        setUniformf("scaleY", configPtr->getScaleY());
        setUniformi("tessellationFactor", configPtr->getTessellationFactor());
        setUniformf("tessellationSlope", configPtr->getTessellationSlope());
        setUniformf("tessellationShift", configPtr->getTessellationShift());
        setUniformi("tbnRange", configPtr->getTbnRange());

        model::activeTexture(0);
        configPtr->getHeightMap()->bind();
        setUniformi("heightMap", 0);

        model::activeTexture(1);
        configPtr->getNormalMap()->bind();
        setUniformi("normalMap", 1);

        model::activeTexture(2);
        configPtr->getSplatMap()->bind();
        setUniformi("splatMap", 2);

        setUniformi("lodMorphArea", configPtr->getLodMorphingArea()[nodePtr->getLod() - 1]);

        int textureUnit = 3;
        for (int i = 0; i < kBiomeCount; ++i) {
            model::Material& material = configPtr->getBiomes()[i]->getMaterial();

            model::activeTexture(textureUnit);
            material.diffuseMap->bind();
            setUniformi("materials[" + std::to_string(i) + "].diffuseMap", textureUnit);
            ++textureUnit;

            model::activeTexture(textureUnit);
            material.normalMap->bind();
            setUniformi("materials[" + std::to_string(i) + "].normalMap", textureUnit);
            ++textureUnit;

            model::activeTexture(textureUnit);
            material.displaceMap->bind();
            setUniformi("materials[" + std::to_string(i) + "].displaceMap", textureUnit);
            ++textureUnit;

            setUniformf("materials[" + std::to_string(i) + "].heightScaling", material.displaceScale);
            setUniformf("materials[" + std::to_string(i) + "].horizontalScaling", material.horizontalScale);
        }
    }

}
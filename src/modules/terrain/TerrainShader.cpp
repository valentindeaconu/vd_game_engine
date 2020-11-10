 //
// Created by Vali on 9/21/2020.
//

#include "TerrainShader.hpp"

namespace mod::terrain {

    TerrainShader::TerrainShader()
        : vd::shader::Shader()
        , kBiomeCount(5)
        , kMaxLights(2)
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

        addUniform("lightView");
        addUniform("lightProjection");

        addUniform("shadowDistance");
        addUniform("shadowTransitionDistance");

        addUniform("cameraPosition");

        addUniform("scaleY");
        addUniform("highDetailRange");

        addUniform("tessellationLevel");

        addUniform("heightMap");
        addUniform("normalMap");
        addUniform("splatMap");
        addUniform("shadowMap");

        addUniform("tessFactor");

        for (int i = 0; i < kBiomeCount; ++i) {
            addUniform("materials[" + std::to_string(i) + "].diffuseMap");
            addUniform("materials[" + std::to_string(i) + "].normalMap");
            addUniform("materials[" + std::to_string(i) + "].displaceMap");
            addUniform("materials[" + std::to_string(i) + "].horizontalScaling");
            addUniform("materials[" + std::to_string(i) + "].heightScaling");
        }

        addUniform("fogDensity");
        addUniform("fogGradient");
        addUniform("fogColor");

        for (size_t i = 0; i < kMaxLights; ++i) {
            std::string currentLightUniformNameBase = "lights[" + std::to_string(i) + "]";

            addUniform(currentLightUniformNameBase + ".position");
            addUniform(currentLightUniformNameBase + ".color");
            addUniform(currentLightUniformNameBase + ".attenuation");
            addUniform(currentLightUniformNameBase + ".ambientStrength");
            addUniform(currentLightUniformNameBase + ".specularStrength");
            addUniform(currentLightUniformNameBase + ".shininess");
        }

        addUniform("clipPlane");
    }

    TerrainShader::~TerrainShader() = default;

    void TerrainShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) {
        auto& enginePtr = entityPtr->getParentEngine();
        setUniform("view", enginePtr->getCamera()->getViewMatrix());
        setUniform("projection", enginePtr->getWindow()->getProjectionMatrix());

        setUniform("cameraPosition", enginePtr->getCamera()->getPosition());

        auto shadowManagerPtr = entityPtr->getParentEngine()->getShadowManager();
        setUniformf("shadowDistance", shadowManagerPtr->getDistance());
        setUniformf("shadowTransitionDistance", shadowManagerPtr->getTransitionDistance());

        setUniform("lightView", shadowManagerPtr->getViewMatrix());
        setUniform("lightProjection", shadowManagerPtr->getProjectionMatrix());

        vd::model::activeTexture(0);
        shadowManagerPtr->getShadowTexture()->bind();
        setUniformi("shadowMap", 0);

        const auto& terrainPtr = std::dynamic_pointer_cast<Terrain>(entityPtr);
        const auto& configPtr = terrainPtr->GetTerrainConfig();

        setUniformf("scaleY", configPtr->getScaleY());
        setUniformi("highDetailRange", configPtr->getHighDetailRange());

        setUniformf("tessellationLevel", configPtr->getTessellationOuterLevel());

        vd::model::activeTexture(1);
        configPtr->getHeightMap()->bind();
        setUniformi("heightMap", 1);

        vd::model::activeTexture(2);
        configPtr->getNormalMap()->bind();
        setUniformi("normalMap", 2);

        vd::model::activeTexture(3);
        configPtr->getSplatMap()->bind();
        setUniformi("splatMap", 3);

        int textureUnit = 4;
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

            vd::model::activeTexture(textureUnit);
            material.displaceMap->bind();
            setUniformi("materials[" + std::to_string(i) + "].displaceMap", textureUnit);
            ++textureUnit;

            setUniformf("materials[" + std::to_string(i) + "].horizontalScaling", material.horizontalScale);

            setUniformf("materials[" + std::to_string(i) + "].heightScaling", material.displaceScale);
        }

        setUniform("clipPlane", enginePtr->getClipPlane());

        static bool loadedBasics = false;
        if (!loadedBasics)
        {
            auto& engineConfigPtr = enginePtr->getEngineConfig();
            setUniformf("fogDensity", engineConfigPtr->getFogDensity());
            setUniformf("fogGradient", engineConfigPtr->getFogGradient());
            setUniform("fogColor", engineConfigPtr->getFogColor());

            auto& lights = engineConfigPtr->getLights();
            for (size_t i = 0; i < kMaxLights; ++i)
            {
                if (i < lights.size())
                {
                    auto& lightPtr = lights[i];

                    std::string currentLightUniformNameBase = "lights[" + std::to_string(i) + "]";

                    setUniform(currentLightUniformNameBase + ".position", lightPtr->getPosition());
                    setUniform(currentLightUniformNameBase + ".color", lightPtr->getColor());
                    setUniform(currentLightUniformNameBase + ".attenuation", lightPtr->getAttenuation());
                    setUniformf(currentLightUniformNameBase + ".ambientStrength", lightPtr->getAmbientStrength());
                    setUniformf(currentLightUniformNameBase + ".specularStrength", lightPtr->getSpecularStrength());
                    setUniformf(currentLightUniformNameBase + ".shininess", lightPtr->getShininess());
                }
            }

            loadedBasics = true;
        }
    }

}
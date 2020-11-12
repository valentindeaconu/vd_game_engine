 //
// Created by Vali on 9/21/2020.
//

#include "TerrainShader.hpp"

namespace mod::terrain {

    TerrainShader::TerrainShader()
        : vd::shader::Shader()
        , kBiomeCount(5)
        , kMaxLights(1)
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

        addUniform("sun.direction");
        addUniform("sun.color");
        addUniform("sun.ambientStrength");
        addUniform("sun.specularStrength");
        addUniform("sun.shininess");

        for (size_t i = 0; i < kMaxLights; ++i) {
            std::string currentLightUniformNameBase = "lights[" + std::to_string(i) + "]";

            addUniform(currentLightUniformNameBase + ".type");
            addUniform(currentLightUniformNameBase + ".position");
            addUniform(currentLightUniformNameBase + ".direction");
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
        setUniform("worldModel", entityPtr->GetWorldTransform().Get());

        auto& enginePtr = vd::ObjectOfType<vd::Engine>::Find();
        setUniform("view", enginePtr->getCamera()->getViewMatrix());
        setUniform("projection", enginePtr->getWindow()->getProjectionMatrix());

        setUniform("cameraPosition", enginePtr->getCamera()->getPosition());

        auto shadowManagerPtr = enginePtr->getShadowManager();
        setUniformf("shadowDistance", shadowManagerPtr->getDistance());
        setUniformf("shadowTransitionDistance", shadowManagerPtr->getTransitionDistance());

        setUniform("lightView", shadowManagerPtr->getViewMatrix());
        setUniform("lightProjection", shadowManagerPtr->getProjectionMatrix());

        vd::model::activeTexture(0);
        shadowManagerPtr->getShadowTexture()->bind();
        setUniformi("shadowMap", 0);

        const auto& terrainPtr = std::dynamic_pointer_cast<Terrain>(entityPtr);
        const auto& propsPtr = terrainPtr->GetProperties();

        setUniformf("scaleY", propsPtr->Get<float>("ScaleY"));
        setUniformi("highDetailRange", propsPtr->Get<int>("HighDetailRange"));

        setUniformf("tessellationLevel", propsPtr->Get<float>("TessellationLevel"));

        vd::model::activeTexture(1);
        terrainPtr->GetHeightMap()->bind();
        setUniformi("heightMap", 1);

        vd::model::activeTexture(2);
        terrainPtr->GetNormalMap()->bind();
        setUniformi("normalMap", 2);

        vd::model::activeTexture(3);
        terrainPtr->GetSplatMap()->bind();
        setUniformi("splatMap", 3);

        int textureUnit = 4;
        for (int i = 0; i < kBiomeCount; ++i) {
            vd::model::Material& material = terrainPtr->GetBiomes()[i]->getMaterial();

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
            auto& propertiesPtr = vd::ObjectOfType<vd::misc::Properties>::Find();

            setUniformf("fogDensity", propertiesPtr->Get<float>("Fog.Density"));
            setUniformf("fogGradient", propertiesPtr->Get<float>("Fog.Gradient"));
            setUniform("fogColor", propertiesPtr->Get<glm::vec3>("Fog.Color"));

            auto& lightManager = vd::ObjectOfType<vd::light::LightManager>::Find();
            auto& sunPtr = lightManager->GetSun();
            setUniform("sun.direction", sunPtr->GetDirection());
            setUniform("sun.color", sunPtr->GetColor());
            setUniformf("sun.ambientStrength", sunPtr->GetAmbientStrength());
            setUniformf("sun.specularStrength", sunPtr->GetSpecularStrength());
            setUniformf("sun.shininess", sunPtr->GetShininess());

            auto& lights = lightManager->GetLights();
            for (size_t i = 0; i < kMaxLights; ++i) {
                if (i < lights.size()) {
                    auto& lightPtr = lights[i];

                    std::string currentLightUniformNameBase = "lights[" + std::to_string(i) + "]";

                    switch (lightPtr->GetType()) {
                        case vd::light::eDirectional: {
                            setUniformi(currentLightUniformNameBase + ".type", 0);
                            setUniform(currentLightUniformNameBase + ".direction", lightPtr->GetDirection());
                            break;
                        }
                        case vd::light::ePoint: {
                            setUniformi(currentLightUniformNameBase + ".type", 1);
                            setUniform(currentLightUniformNameBase + ".position", lightPtr->GetPosition());
                            break;
                        }
                        case vd::light::eSpot:
                        default: {
                            setUniformi(currentLightUniformNameBase + ".type", 2);
                            setUniform(currentLightUniformNameBase + ".position", lightPtr->GetPosition());
                            break;
                        }
                    }
                    setUniform(currentLightUniformNameBase + ".color", lightPtr->GetColor());
                    setUniform(currentLightUniformNameBase + ".attenuation", lightPtr->GetAttenuation());
                    setUniformf(currentLightUniformNameBase + ".ambientStrength", lightPtr->GetAmbientStrength());
                    setUniformf(currentLightUniformNameBase + ".specularStrength", lightPtr->GetSpecularStrength());
                    setUniformf(currentLightUniformNameBase + ".shininess", lightPtr->GetShininess());
                }
            }

            loadedBasics = true;
        }
    }

}
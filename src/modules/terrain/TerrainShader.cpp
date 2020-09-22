#include "TerrainShader.hpp"

namespace mod::terrain
{
    TerrainShader::TerrainShader()
        : vd::shader::Shader()
        , kMaxTextures(6)
        , kMaxLights(2)
    {
        loadAndAddShader("./resources/shaders/terrain/terrain_VS.glsl", vd::shader::eVertexShader);
        loadAndAddShader("./resources/shaders/terrain/terrain_FS.glsl", vd::shader::eFragmentShader);

        compileShader();

        addUniform("model");
        addUniform("view");
        addUniform("projection");

        addUniform("lightView");
        addUniform("lightProjection");

        addUniform("shadowDistance");
        addUniform("shadowTransitionDistance");

        addUniform("splatMap");
        addUniform("shadowMap");

        for (int i = 0; i < kMaxTextures; ++i)
        {
            addUniform("textures[" + std::to_string(i) + "]");
        }

        addUniform("fogDensity");
        addUniform("fogGradient");
        addUniform("fogColor");

        for (size_t i = 0; i < kMaxLights; ++i)
        {
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

    void TerrainShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex)
    {
        TerrainPtr terrainPtr = std::dynamic_pointer_cast<Terrain>(entityPtr);

        setUniform("model", terrainPtr->getLocalTransform().get());

        auto& enginePtr = entityPtr->getParentEngine();
        setUniform("view", enginePtr->getCamera()->getViewMatrix());
        setUniform("projection", enginePtr->getWindow()->getProjectionMatrix());

        setUniform("lightView", enginePtr->getShadowManager()->getViewMatrix());
        setUniform("lightProjection", enginePtr->getShadowManager()->getProjectionMatrix());

        auto shadowManagerPtr = entityPtr->getParentEngine()->getShadowManager();
        setUniformf("shadowDistance", shadowManagerPtr->getDistance());
        setUniformf("shadowTransitionDistance", shadowManagerPtr->getTransitionDistance());

        vd::model::activeTexture(0);
        shadowManagerPtr->getShadowTexture()->bind();
        setUniformi("shadowMap", 0);

        auto terrainConfig = terrainPtr->getTerrainConfig();

        vd::model::activeTexture(1);
        terrainConfig->getSplatmap()->bind();
        setUniformi("splatMap", 1);

        const size_t textureUnit = 2;
        for (size_t i = 0; i < kMaxTextures; ++i)
        {
            vd::model::activeTexture(textureUnit + i);
            terrainConfig->getBiomeAtlas()[i].material.diffuseMap->bind();
            setUniformi("textures[" + std::to_string(i) + "]", textureUnit + i);
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
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

        addUniform("splatmap");

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
    }

    TerrainShader::~TerrainShader() = default;

    void TerrainShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex)
    {
        TerrainPtr terrainPtr = std::dynamic_pointer_cast<Terrain>(entityPtr);

        setUniform("model", terrainPtr->getLocalTransform().get());
        setUniform("view", terrainPtr->getParentEngine()->getCamera()->getViewMatrix());
        setUniform("projection", terrainPtr->getParentEngine()->getWindow()->getProjectionMatrix());

        auto terrainConfig = terrainPtr->getTerrainConfig();

        static bool loadedBasics = false;


        if (loadedBasics == false)
        {
            vd::model::activeTexture(1);
            terrainConfig->getSplatmap()->bind();
            setUniformi("splatmap", 1);

            const size_t textureUnit = 2;
            for (size_t i = 0; i < kMaxTextures; ++i)
            {
                vd::model::activeTexture(textureUnit + i);
                terrainConfig->getBiomeAtlas()[i].material.diffusemap->bind();
                setUniformi("textures[" + std::to_string(i) + "]", textureUnit + i);
            }

            auto& enginePtr = entityPtr->getParentEngine();
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
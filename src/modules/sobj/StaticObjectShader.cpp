#include "StaticObjectShader.hpp"

namespace mod::sobj
{
    StaticObjectShader::StaticObjectShader()
        : vd::shader::Shader()
        , kMaxLights(2)
    {
        loadAndAddShader("./resources/shaders/entity/entity_VS.glsl", vd::shader::eVertexShader);
        loadAndAddShader("./resources/shaders/entity/entity_FS.glsl", vd::shader::eFragmentShader);
        compileShader();

        addUniform("model");
        addUniform("view");
        addUniform("projection");

        addUniform("diffuseMap");
        addUniform("specularMap");

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

    StaticObjectShader::~StaticObjectShader() = default;

    void StaticObjectShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex)
    {
        setUniform("model", entityPtr->getWorldTransform().get());

        auto enginePtr = entityPtr->getParentEngine();
        setUniform("view", enginePtr->getCamera()->getViewMatrix());
        setUniform("projection", enginePtr->getWindow()->getProjectionMatrix());

        vd::model::MeshPtr& meshPtr = entityPtr->getMeshes()[meshIndex];

        if (!meshPtr->materials.empty())
        {
            vd::model::Material& meshMaterial = meshPtr->materials.front();

            if (meshMaterial.diffuseMap != nullptr)
            {
                vd::model::activeTexture(0);
                meshMaterial.diffuseMap->bind();
                setUniformi("diffusemap", 0);
            }

            if (meshMaterial.specularMap != nullptr)
            {
                vd::model::activeTexture(1);
                meshMaterial.specularMap->bind();
                setUniformi("specularmap", 1);
            }
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
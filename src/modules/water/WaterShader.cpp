//
// Created by Vali on 9/11/2020.
//

#include "WaterShader.hpp"

namespace mod::water {

    WaterShader::WaterShader()
        : vd::shader::Shader()
        , kMaxLights(2)
    {
        loadAndAddShader("./resources/shaders/water/water_VS.glsl", vd::shader::eVertexShader);
        loadAndAddShader("./resources/shaders/water/water_FS.glsl", vd::shader::eFragmentShader);

        compileShader();

        addUniform("model");
        addUniform("view");
        addUniform("projection");

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

    WaterShader::~WaterShader() = default;

    void WaterShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) {
        WaterPtr waterPtr = std::dynamic_pointer_cast<Water>(entityPtr);

        setUniform("model", waterPtr->getLocalTransform().get());

        auto& enginePtr = entityPtr->getParentEngine();
        setUniform("view", enginePtr->getCamera()->getViewMatrix());
        setUniform("projection", enginePtr->getWindow()->getProjectionMatrix());

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
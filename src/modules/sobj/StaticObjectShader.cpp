#include "StaticObjectShader.hpp"

namespace mod::sobj {

    StaticObjectShader::StaticObjectShader()
        : vd::shader::Shader()
        , kMaxLights(1)
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

    StaticObjectShader::~StaticObjectShader() = default;

    void StaticObjectShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) {
        setUniform("model", entityPtr->GetWorldTransform().get());

        auto& enginePtr = vd::ObjectOfType<vd::Engine>::Find();
        setUniform("view", enginePtr->getCamera()->getViewMatrix());
        setUniform("projection", enginePtr->getWindow()->getProjectionMatrix());

        vd::model::MeshPtr& meshPtr = entityPtr->GetMeshes()[meshIndex];

        if (!meshPtr->materials.empty()) {
            vd::model::Material& meshMaterial = meshPtr->materials.front();

            if (meshMaterial.diffuseMap != nullptr) {
                vd::model::activeTexture(0);
                meshMaterial.diffuseMap->bind();
                setUniformi("diffuseMap", 0);
            }

            if (meshMaterial.specularMap != nullptr) {
                vd::model::activeTexture(1);
                meshMaterial.specularMap->bind();
                setUniformi("specularMap", 1);
            }
        }

        setUniform("clipPlane", enginePtr->getClipPlane());

        static bool loadedBasics = false;
        if (!loadedBasics) {
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
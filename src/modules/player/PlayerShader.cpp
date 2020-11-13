#include "PlayerShader.hpp"

namespace mod::player
{
    PlayerShader::PlayerShader()
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

        addUniform("transparency");

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

    PlayerShader::~PlayerShader() = default;

    void PlayerShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) {
        setUniform("model", entityPtr->WorldTransform().Get());

        setUniform("view", vd::ObjectOfType<vd::camera::Camera>::Find()->ViewMatrix());
        setUniform("projection", vd::ObjectOfType<vd::window::Window>::Find()->ProjectionMatrix());

        vd::model::MeshPtr& meshPtr = entityPtr->Meshes()[meshIndex];

        setUniformi("transparency", 0);

        if (!meshPtr->materials.empty()) {
            vd::model::Material& meshMaterial = meshPtr->materials.front();

            if (meshMaterial.diffuseMap != nullptr) {
                vd::gl::ActiveTexture(0);
                meshMaterial.diffuseMap->Bind();
                setUniformi("diffuseMap", 0);
            }

            if (meshMaterial.specularMap != nullptr) {
                vd::gl::ActiveTexture(1);
                meshMaterial.specularMap->Bind();
                setUniformi("specularMap", 1);
            }
        }

        setUniform("clipPlane", vd::ObjectOfType<vd::Engine>::Find()->getClipPlane());

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
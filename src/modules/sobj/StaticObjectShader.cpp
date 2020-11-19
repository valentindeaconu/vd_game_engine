#include "StaticObjectShader.hpp"

namespace mod::sobj {

    StaticObjectShader::StaticObjectShader()
        : vd::gl::Shader()
    {
        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/entity/entity_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/entity/entity_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
    }

    StaticObjectShader::~StaticObjectShader() = default;

    void StaticObjectShader::Link() {
        m_pProperties = vd::ObjectOfType<vd::property::GlobalProperties>::Find();
        m_pContext = vd::ObjectOfType<vd::kernel::Context>::Find();
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
        m_pLightManager = vd::ObjectOfType<vd::light::LightManager>::Find();
    }

    void StaticObjectShader::AddUniforms() {
        AddUniform("model");
        AddUniform("view");
        AddUniform("projection");

        AddUniform("diffuseMap");
        AddUniform("specularMap");

        AddUniform("fogDensity");
        AddUniform("fogGradient");
        AddUniform("fogColor");

        AddUniform("sun.direction");
        AddUniform("sun.color");
        AddUniform("sun.ambientStrength");
        AddUniform("sun.specularStrength");
        AddUniform("sun.shininess");

        for (size_t i = 0; i < m_pLightManager->Lights().size(); ++i) {
            std::string currentLightUniformNameBase = "lights[" + std::to_string(i) + "]";

            AddUniform(currentLightUniformNameBase + ".type");
            AddUniform(currentLightUniformNameBase + ".position");
            AddUniform(currentLightUniformNameBase + ".direction");
            AddUniform(currentLightUniformNameBase + ".color");
            AddUniform(currentLightUniformNameBase + ".attenuation");
            AddUniform(currentLightUniformNameBase + ".ambientStrength");
            AddUniform(currentLightUniformNameBase + ".specularStrength");
            AddUniform(currentLightUniformNameBase + ".shininess");
        }

        AddUniform("clipPlane");
    }

    void StaticObjectShader::InitUniforms(vd::object::EntityPtr pEntity) {
        AddUniforms();

        SetUniform("fogDensity", m_pProperties->Get<float>("Fog.Density"));
        SetUniform("fogGradient", m_pProperties->Get<float>("Fog.Gradient"));
        SetUniform("fogColor", m_pProperties->Get<glm::vec3>("Fog.Color"));

        auto& pSun = m_pLightManager->Sun();
        SetUniform("sun.direction", pSun->Direction());
        SetUniform("sun.color", pSun->Color());
        SetUniform("sun.ambientStrength", pSun->AmbientStrength());
        SetUniform("sun.specularStrength", pSun->SpecularStrength());
        SetUniform("sun.shininess", pSun->Shininess());

        auto& lights = m_pLightManager->Lights();
        for (size_t i = 0; i < lights.size(); ++i) {
            if (i < lights.size()) {
                auto& lightPtr = lights[i];

                std::string currentLightUniformNameBase = "lights[" + std::to_string(i) + "]";

                switch (lightPtr->Type()) {
                    case vd::light::eDirectional: {
                        SetUniform(currentLightUniformNameBase + ".type", 0);
                        SetUniform(currentLightUniformNameBase + ".direction", lightPtr->Direction());
                        break;
                    }
                    case vd::light::ePoint: {
                        SetUniform(currentLightUniformNameBase + ".type", 1);
                        SetUniform(currentLightUniformNameBase + ".position", lightPtr->Position());
                        break;
                    }
                    case vd::light::eSpot:
                    default: {
                        SetUniform(currentLightUniformNameBase + ".type", 2);
                        SetUniform(currentLightUniformNameBase + ".position", lightPtr->Position());
                        break;
                    }
                }
                SetUniform(currentLightUniformNameBase + ".color", lightPtr->Color());
                SetUniform(currentLightUniformNameBase + ".attenuation", lightPtr->Attenuation());
                SetUniform(currentLightUniformNameBase + ".ambientStrength", lightPtr->AmbientStrength());
                SetUniform(currentLightUniformNameBase + ".specularStrength", lightPtr->SpecularStrength());
                SetUniform(currentLightUniformNameBase + ".shininess", lightPtr->Shininess());
            }
        }
    }


    void StaticObjectShader::UpdateUniforms(vd::object::EntityPtr pEntity, uint32_t meshIndex) {
        SetUniform("model", pEntity->WorldTransform().Get());

        SetUniform("view", m_pCamera->ViewMatrix());
        SetUniform("projection", m_pWindow->ProjectionMatrix());

        vd::model::MeshPtr& meshPtr = pEntity->Meshes()[meshIndex];

        if (!meshPtr->materials.empty()) {
            vd::model::Material& meshMaterial = meshPtr->materials.front();

            if (meshMaterial.diffuseMap != nullptr) {
                vd::gl::ActiveTexture(0);
                meshMaterial.diffuseMap->Bind();
                SetUniform("diffuseMap", 0);
            }

            if (meshMaterial.specularMap != nullptr) {
                vd::gl::ActiveTexture(1);
                meshMaterial.specularMap->Bind();
                SetUniform("specularMap", 1);
            }
        }

        SetUniform("clipPlane", m_pContext->ClipPlane());
    }
}
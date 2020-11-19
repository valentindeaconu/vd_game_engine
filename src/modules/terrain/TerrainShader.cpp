 //
// Created by Vali on 9/21/2020.
//

#include "TerrainShader.hpp"

namespace mod::terrain {

    TerrainShader::TerrainShader()
        : vd::gl::Shader()
    {
        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/terrain/terrain_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string tcSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/terrain/terrain_TC.glsl", tcSource);
        AddShader(tcSource, vd::gl::Shader::eTessellationControlShader);

        std::string teSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/terrain/terrain_TE.glsl", teSource);
        AddShader(teSource, vd::gl::Shader::eTessellationEvaluationShader);

        std::string gsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/terrain/terrain_GS.glsl", gsSource);
        AddShader(gsSource, vd::gl::Shader::eGeometryShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/terrain/terrain_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
    }

    TerrainShader::~TerrainShader() = default;

    void TerrainShader::Link() {
        m_pProperties = vd::ObjectOfType<vd::property::GlobalProperties>::Find();
        m_pContext = vd::ObjectOfType<vd::kernel::Context>::Find();
        m_pLightManager = vd::ObjectOfType<vd::light::LightManager>::Find();
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
        m_pShadowManager = vd::ObjectOfType<mod::shadow::ShadowManager>::Find();
    }

    void TerrainShader::AddUniforms() {
        AddUniform("localModel");
        AddUniform("worldModel");
        AddUniform("view");
        AddUniform("projection");

        AddUniform("lightView");
        AddUniform("lightProjection");

        AddUniform("shadowDistance");
        AddUniform("shadowTransitionDistance");

        AddUniform("cameraPosition");

        AddUniform("scaleY");
        AddUniform("highDetailRange");

        AddUniform("tessellationLevel");

        AddUniform("heightMap");
        AddUniform("normalMap");
        AddUniform("splatMap");
        AddUniform("shadowMap");

        AddUniform("tessFactor");

        for (int i = 0; i < m_BiomeCount; ++i) {
            AddUniform("materials[" + std::to_string(i) + "].diffuseMap");
            AddUniform("materials[" + std::to_string(i) + "].normalMap");
            AddUniform("materials[" + std::to_string(i) + "].displaceMap");
            AddUniform("materials[" + std::to_string(i) + "].horizontalScaling");
            AddUniform("materials[" + std::to_string(i) + "].heightScaling");
        }

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

    void TerrainShader::InitUniforms(vd::object::EntityPtr pEntity) {
        m_BiomeCount = std::dynamic_pointer_cast<Terrain>(pEntity)->Biomes().size();

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
                auto& pLight = lights[i];

                std::string currentLightUniformNameBase = "lights[" + std::to_string(i) + "]";

                switch (pLight->Type()) {
                    case vd::light::eDirectional: {
                        SetUniform(currentLightUniformNameBase + ".type", 0);
                        SetUniform(currentLightUniformNameBase + ".direction", pLight->Direction());
                        break;
                    }
                    case vd::light::ePoint: {
                        SetUniform(currentLightUniformNameBase + ".type", 1);
                        SetUniform(currentLightUniformNameBase + ".position", pLight->Position());
                        break;
                    }
                    case vd::light::eSpot:
                    default: {
                        SetUniform(currentLightUniformNameBase + ".type", 2);
                        SetUniform(currentLightUniformNameBase + ".position", pLight->Position());
                        break;
                    }
                }
                SetUniform(currentLightUniformNameBase + ".color", pLight->Color());
                SetUniform(currentLightUniformNameBase + ".attenuation", pLight->Attenuation());
                SetUniform(currentLightUniformNameBase + ".ambientStrength", pLight->AmbientStrength());
                SetUniform(currentLightUniformNameBase + ".specularStrength", pLight->SpecularStrength());
                SetUniform(currentLightUniformNameBase + ".shininess", pLight->Shininess());
            }
        }
    }

    void TerrainShader::UpdateUniforms(vd::object::EntityPtr pEntity, uint32_t meshIndex) {
        SetUniform("worldModel", pEntity->WorldTransform().Get());

        SetUniform("view", m_pCamera->ViewMatrix());
        SetUniform("projection", m_pWindow->ProjectionMatrix());

        SetUniform("cameraPosition", m_pCamera->Position());

        SetUniform("shadowDistance", m_pShadowManager->Distance());
        SetUniform("shadowTransitionDistance", m_pShadowManager->TransitionDistance());

        SetUniform("lightView", m_pShadowManager->ViewMatrix());
        SetUniform("lightProjection", m_pShadowManager->ProjectionMatrix());

        vd::gl::ActiveTexture(0);
        m_pShadowManager->ShadowTexture()->Bind();
        SetUniform("shadowMap", 0);

        const auto& terrainPtr = std::dynamic_pointer_cast<Terrain>(pEntity);
        const auto& propsPtr = terrainPtr->Properties();

        SetUniform("scaleY", propsPtr->Get<float>("ScaleY"));
        SetUniform("highDetailRange", propsPtr->Get<int>("HighDetailRange"));

        SetUniform("tessellationLevel", propsPtr->Get<float>("TessellationLevel"));

        vd::gl::ActiveTexture(1);
        terrainPtr->HeightMap()->Bind();
        SetUniform("heightMap", 1);

        vd::gl::ActiveTexture(2);
        terrainPtr->NormalMap()->Bind();
        SetUniform("normalMap", 2);

        vd::gl::ActiveTexture(3);
        terrainPtr->SplatMap()->Bind();
        SetUniform("splatMap", 3);

        int textureUnit = 4;
        for (int i = 0; i < m_BiomeCount; ++i) {
            vd::model::Material& material = terrainPtr->Biomes()[i]->getMaterial();

            vd::gl::ActiveTexture(textureUnit);
            material.diffuseMap->Bind();
            SetUniform("materials[" + std::to_string(i) + "].diffuseMap", textureUnit);
            ++textureUnit;

            vd::gl::ActiveTexture(textureUnit);
            material.normalMap->Bind();
            SetUniform("materials[" + std::to_string(i) + "].normalMap", textureUnit);
            ++textureUnit;

            vd::gl::ActiveTexture(textureUnit);
            material.displaceMap->Bind();
            SetUniform("materials[" + std::to_string(i) + "].displaceMap", textureUnit);
            ++textureUnit;

            SetUniform("materials[" + std::to_string(i) + "].horizontalScaling", material.horizontalScale);

            SetUniform("materials[" + std::to_string(i) + "].heightScaling", material.displaceScale);
        }

        SetUniform("clipPlane", m_pContext->ClipPlane());
    }

}
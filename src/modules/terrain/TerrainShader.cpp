 //
// Created by Vali on 9/21/2020.
//

#include "TerrainShader.hpp"

namespace mod::terrain {

    TerrainShader::TerrainShader() : vd::component::IEntity2DShader() {
        Create();
        
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
        m_pContext = vd::ObjectOfType<vd::context::Context>::Find();
        m_pLightManager = vd::ObjectOfType<vd::light::LightManager>::Find();
        m_pFogManager = vd::ObjectOfType<vd::fog::FogManager>::Find();
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

        AddUniform("clipPlane");

        AddUniform("isWireframe");
        AddUniform("wireframeColor");


        m_pFogManager->AddUniforms(shared_from_this());
        m_pLightManager->AddUniforms(shared_from_this());

    }

    void TerrainShader::InitUniforms(vd::object::Entity2DPtr pEntity) {
        m_BiomeCount = std::dynamic_pointer_cast<Terrain>(pEntity)->Biomes().size();

        AddUniforms();

        m_pFogManager->SetUniforms(shared_from_this());
        m_pLightManager->SetUniforms(shared_from_this());
    }

    void TerrainShader::UpdateUniforms(vd::object::Entity2DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) {
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
            vd::model::Material& material = terrainPtr->Biomes()[i]->Material();

            vd::gl::ActiveTexture(textureUnit);
            material.DiffuseMap()->Bind();
            SetUniform("materials[" + std::to_string(i) + "].diffuseMap", textureUnit);
            ++textureUnit;

            vd::gl::ActiveTexture(textureUnit);
            material.NormalMap()->Bind();
            SetUniform("materials[" + std::to_string(i) + "].normalMap", textureUnit);
            ++textureUnit;

            vd::gl::ActiveTexture(textureUnit);
            material.DisplaceMap()->Bind();
            SetUniform("materials[" + std::to_string(i) + "].displaceMap", textureUnit);
            ++textureUnit;

            SetUniform("materials[" + std::to_string(i) + "].horizontalScaling", material.HorizontalScale());

            SetUniform("materials[" + std::to_string(i) + "].heightScaling", material.DisplaceScale());
        }

        SetUniform("clipPlane", m_pContext->ClipPlane());

        SetUniform("isWireframe", m_pContext->WireframeMode());
        SetUniform("wireframeColor", glm::vec4(0.1f, 1.0f, 0.1f, 1.0f));
    }

}
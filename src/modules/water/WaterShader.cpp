//
// Created by Vali on 9/11/2020.
//

#include "WaterShader.hpp"

namespace mod::water {

    void WaterShader::Link() {
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
        m_pLightManager = vd::ObjectOfType<vd::light::LightManager>::Find();
    }

    void WaterShader::Init() {
        Create();

        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/water/water_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/water/water_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();

        AddUniform("model");
        AddUniform("view");
        AddUniform("projection");

        AddUniform("cameraPosition");

        AddUniform("tiling");
        AddUniform("waveStrength");
        AddUniform("moveFactor");

        AddUniform("reflectionTexture");
        AddUniform("refractionTexture");

        AddUniform("dudvMap");
        AddUniform("normalMap");
        AddUniform("depthMap");

        AddUniform("sunDirection");
        AddUniform("sunColor");
        AddUniform("shineDamper");
        AddUniform("reflectivity");
        AddUniform("baseColor");

        AddUniform("nearPlane");
        AddUniform("farPlane");
    }

    void WaterShader::InitUniforms(vd::object::Entity3DPtr pEntity) {
        WaterPtr pWater = std::dynamic_pointer_cast<Water>(pEntity);
        auto& pProperties = pWater->Properties();

        // TODO: Move those to the Water Class
        SetUniform("tiling", pProperties->Get<float>("Tiling"));
        SetUniform("waveStrength", pProperties->Get<float>("Wave.Strength"));
        SetUniform("shineDamper", pProperties->Get<float>("ShineDamper"));
        SetUniform("reflectivity", pProperties->Get<float>("Reflectivity"));
        SetUniform("baseColor", pProperties->Get<glm::vec3>("BaseColor"));
    }

    void WaterShader::UpdateUniforms(vd::object::Entity3DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) {
        WaterPtr pWater = std::dynamic_pointer_cast<Water>(pEntity);

        SetUniform("model", pEntity->LocalTransform().Get());

        SetUniform("view", m_pCamera->ViewMatrix());
        SetUniform("projection", m_pWindow->ProjectionMatrix());

        SetUniform("cameraPosition", m_pCamera->Position());

        SetUniform("nearPlane", m_pWindow->NearPlane());
        SetUniform("farPlane", m_pWindow->FarPlane());

        pWater->ReflectionFramebuffer()->ColorTexture()->BindToUnit(0);
        SetUniform("reflectionTexture", 0);

        pWater->RefractionFramebuffer()->ColorTexture()->BindToUnit(1);
        SetUniform("refractionTexture", 1);

        pWater->RefractionFramebuffer()->DepthTexture()->BindToUnit(2);
        SetUniform("depthMap", 2);

        auto& pSun = m_pLightManager->Sun();

        SetUniform("sunDirection", pSun->Direction());
        SetUniform("sunColor", pSun->Color());

        SetUniform("moveFactor", pWater->GetMoveFactor());

        auto& waterMaterial = pWater->Material();
        waterMaterial.DisplaceMap()->BindToUnit(3);
        SetUniform("dudvMap", 3);

        waterMaterial.NormalMap()->BindToUnit(4);
        SetUniform("normalMap", 4);
    }


}
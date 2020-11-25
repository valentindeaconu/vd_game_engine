//
// Created by Vali on 9/11/2020.
//

#include "WaterShader.hpp"

namespace mod::water {

    WaterShader::WaterShader()
        : vd::gl::IEntityShader()
    {
        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/water/water_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/water/water_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
    }

    WaterShader::~WaterShader() = default;

    void WaterShader::Link() {
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
        m_pLightManager = vd::ObjectOfType<vd::light::LightManager>::Find();
    }

    void WaterShader::AddUniforms() {
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

    void WaterShader::InitUniforms(vd::object::EntityPtr pEntity) {
        AddUniforms();

        WaterPtr pWater = std::dynamic_pointer_cast<Water>(pEntity);
        auto& pProperties = pWater->Properties();

        SetUniform("tiling", pProperties->Get<float>("Tiling"));
        SetUniform("waveStrength", pProperties->Get<float>("Wave.Strength"));
        SetUniform("shineDamper", pProperties->Get<float>("ShineDamper"));
        SetUniform("reflectivity", pProperties->Get<float>("Reflectivity"));
        SetUniform("baseColor", pProperties->Get<glm::vec3>("BaseColor"));
    }

    void WaterShader::UpdateUniforms(vd::object::EntityPtr pEntity, uint32_t meshIndex) {
        WaterPtr pWater = std::dynamic_pointer_cast<Water>(pEntity);

        SetUniform("model", pEntity->LocalTransform().Get());

        SetUniform("view", m_pCamera->ViewMatrix());
        SetUniform("projection", m_pWindow->ProjectionMatrix());

        SetUniform("cameraPosition", m_pCamera->Position());

        SetUniform("nearPlane", m_pWindow->NearPlane());
        SetUniform("farPlane", m_pWindow->FarPlane());

        vd::gl::ActiveTexture(0);
        pWater->ReflectionFramebuffer()->GetColorTexture()->Bind();
        SetUniform("reflectionTexture", 0);

        vd::gl::ActiveTexture(1);
        pWater->RefractionFramebuffer()->GetColorTexture()->Bind();
        SetUniform("refractionTexture", 1);

        vd::gl::ActiveTexture(2);
        pWater->RefractionFramebuffer()->GetDepthTexture()->Bind();
        SetUniform("depthMap", 2);

        auto& pSun = m_pLightManager->Sun();

        SetUniform("sunDirection", pSun->Direction());
        SetUniform("sunColor", pSun->Color());

        SetUniform("moveFactor", pWater->GetMoveFactor());

        auto& waterMaterial = pWater->Material();
        vd::gl::ActiveTexture(3);
        waterMaterial.displaceMap->Bind();
        SetUniform("dudvMap", 3);

        vd::gl::ActiveTexture(4);
        waterMaterial.normalMap->Bind();
        SetUniform("normalMap", 4);
    }


}
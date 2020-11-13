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

        addUniform("cameraPosition");

        addUniform("tiling");
        addUniform("waveStrength");
        addUniform("moveFactor");

        addUniform("reflectionTexture");
        addUniform("refractionTexture");

        addUniform("dudvMap");
        addUniform("normalMap");
        addUniform("depthMap");

        addUniform("sunDirection");
        addUniform("sunColor");
        addUniform("shineDamper");
        addUniform("reflectivity");
        addUniform("baseColor");

        addUniform("nearPlane");
        addUniform("farPlane");
    }

    WaterShader::~WaterShader() = default;

    void WaterShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) {
        WaterPtr waterPtr = std::dynamic_pointer_cast<Water>(entityPtr);

        setUniform("model", waterPtr->LocalTransform().Get());

        setUniform("view", vd::ObjectOfType<vd::camera::Camera>::Find()->ViewMatrix());
        setUniform("projection", vd::ObjectOfType<vd::window::Window>::Find()->ProjectionMatrix());

        setUniform("cameraPosition", vd::ObjectOfType<vd::camera::Camera>::Find()->Position());

        setUniformf("nearPlane", vd::ObjectOfType<vd::window::Window>::Find()->NearPlane());
        setUniformf("farPlane", vd::ObjectOfType<vd::window::Window>::Find()->FarPlane());

        vd::gl::ActiveTexture(0);
        waterPtr->GetReflectionFramebuffer()->GetColorTexture()->Bind();
        setUniformi("reflectionTexture", 0);

        vd::gl::ActiveTexture(1);
        waterPtr->GetRefractionFramebuffer()->GetColorTexture()->Bind();
        setUniformi("refractionTexture", 1);

        vd::gl::ActiveTexture(2);
        waterPtr->GetRefractionFramebuffer()->GetDepthTexture()->Bind();
        setUniformi("depthMap", 2);

        auto& lightManager = vd::ObjectOfType<vd::light::LightManager>::Find();
        auto& sunPtr = lightManager->GetSun();

        setUniform("sunDirection", sunPtr->GetDirection());
        setUniform("sunColor", sunPtr->GetColor());

        setUniformf("moveFactor", waterPtr->GetMoveFactor());

        auto& waterMaterial = waterPtr->GetMaterial();
        vd::gl::ActiveTexture(3);
        waterMaterial.displaceMap->Bind();
        setUniformi("dudvMap", 3);

        vd::gl::ActiveTexture(4);
        waterMaterial.normalMap->Bind();
        setUniformi("normalMap", 4);

        static bool loadedBasics = false;
        if (!loadedBasics) {
            auto& propsPtr = waterPtr->GetProperties();

            setUniformf("tiling", propsPtr->Get<float>("Tiling"));
            setUniformf("waveStrength", propsPtr->Get<float>("Wave.Strength"));
            setUniformf("shineDamper", propsPtr->Get<float>("ShineDamper"));
            setUniformf("reflectivity", propsPtr->Get<float>("Reflectivity"));
            setUniform("baseColor", propsPtr->Get<glm::vec3>("BaseColor"));

            loadedBasics = true;
        }
    }

}
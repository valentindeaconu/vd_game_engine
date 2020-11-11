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

        setUniform("model", waterPtr->GetLocalTransform().get());

        auto& enginePtr = vd::ObjectOfType<vd::Engine>::Find();
        setUniform("view", enginePtr->getCamera()->getViewMatrix());
        setUniform("projection", enginePtr->getWindow()->getProjectionMatrix());

        setUniform("cameraPosition", enginePtr->getCamera()->getPosition());

        setUniformf("nearPlane", enginePtr->getWindow()->getNearPlane());
        setUniformf("farPlane", enginePtr->getWindow()->getFarPlane());

        vd::model::activeTexture(0);
        waterPtr->GetReflectionFramebuffer()->GetColorTexture()->bind();
        setUniformi("reflectionTexture", 0);

        vd::model::activeTexture(1);
        waterPtr->GetRefractionFramebuffer()->GetColorTexture()->bind();
        setUniformi("refractionTexture", 1);

        vd::model::activeTexture(2);
        waterPtr->GetRefractionFramebuffer()->GetDepthTexture()->bind();
        setUniformi("depthMap", 2);

        auto& lightManager = vd::ObjectOfType<vd::light::LightManager>::Find();
        auto& sunPtr = lightManager->GetSun();

        setUniform("sunDirection", sunPtr->GetDirection());
        setUniform("sunColor", sunPtr->GetColor());

        setUniformf("moveFactor", waterPtr->GetMoveFactor());

        auto& waterMaterial = waterPtr->GetMaterial();
        vd::model::activeTexture(3);
        waterMaterial.displaceMap->bind();
        setUniformi("dudvMap", 3);

        vd::model::activeTexture(4);
        waterMaterial.normalMap->bind();
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
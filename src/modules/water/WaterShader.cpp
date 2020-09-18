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

        addUniform("sunPosition");
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

        setUniform("model", waterPtr->getLocalTransform().get());

        auto& enginePtr = entityPtr->getParentEngine();
        setUniform("view", enginePtr->getCamera()->getViewMatrix());
        setUniform("projection", enginePtr->getWindow()->getProjectionMatrix());

        setUniform("cameraPosition", enginePtr->getCamera()->getPosition());

        setUniformf("nearPlane", enginePtr->getWindow()->getNearPlane());
        setUniformf("farPlane", enginePtr->getWindow()->getFarPlane());

        vd::model::activeTexture(0);
        waterPtr->getReflectionFramebuffer()->getColorTexture()->bind();
        setUniformi("reflectionTexture", 0);

        vd::model::activeTexture(1);
        waterPtr->getRefractionFramebuffer()->getColorTexture()->bind();
        setUniformi("refractionTexture", 1);

        vd::model::activeTexture(2);
        waterPtr->getRefractionFramebuffer()->getDepthTexture()->bind();
        setUniformi("depthMap", 2);

        setUniform("sunPosition", enginePtr->getEngineConfig()->getLights().front()->getPosition());
        setUniform("sunColor", enginePtr->getEngineConfig()->getLights().front()->getColor());

        setUniformf("moveFactor", waterPtr->getMoveFactor());

        auto& waterConfigPtr = waterPtr->getWaterConfig();

        vd::model::activeTexture(3);
        waterConfigPtr->getDuDvMap("Chill")->bind();
        setUniformi("dudvMap", 3);

        vd::model::activeTexture(4);
        waterConfigPtr->getNormalMap("Chill")->bind();
        setUniformi("normalMap", 4);

        static bool loadedBasics = false;
        if (!loadedBasics)
        {
            setUniformf("tiling", waterConfigPtr->getTiling());
            setUniformf("waveStrength", waterConfigPtr->getWaveStrength());
            setUniformf("shineDamper", waterConfigPtr->getShineDamper());
            setUniformf("reflectivity", waterConfigPtr->getReflectivity());

            setUniform("baseColor", waterConfigPtr->getBaseColor());

            loadedBasics = true;
        }
    }

}
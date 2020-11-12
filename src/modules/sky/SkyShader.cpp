#include "SkyShader.hpp"

namespace mod::sky
{
    SkyShader::SkyShader()
        : vd::shader::Shader()
    {
        loadAndAddShader("./resources/shaders/sky/sky_VS.glsl", vd::shader::eVertexShader);
        loadAndAddShader("./resources/shaders/sky/sky_FS.glsl", vd::shader::eFragmentShader);
        compileShader();

        addUniform("view");
        addUniform("projection");

        addUniform("fogDensity");
        addUniform("fogColor");
    }

    SkyShader::~SkyShader() = default;

    void SkyShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) {
        setUniform("view", glm::mat4(glm::mat3(vd::ObjectOfType<vd::camera::ICamera>::Find()->ViewMatrix())));
        setUniform("projection", vd::ObjectOfType<vd::window::Window>::Find()->ProjectionMatrix());

        static bool loadedBasics = false;
        if (!loadedBasics) {
            auto& propertiesPtr = vd::ObjectOfType<vd::misc::Properties>::Find();

            setUniformf("fogDensity", propertiesPtr->Get<float>("Fog.SkyDensity"));
            setUniform("fogColor", propertiesPtr->Get<glm::vec3>("Fog.Color"));

            loadedBasics = true;
        }
    }
}
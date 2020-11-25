//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_SKYSHADER_HPP
#define VD_GAME_ENGINE_SKYSHADER_HPP

#include <engine/api/gl/Shader.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include <engine/injector/Injectable.hpp>

#include <engine/property/GlobalProperties.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/window/Window.hpp>

namespace mod::sky {
    class SkyShader : public vd::gl::IEntityShader, public vd::injector::Injectable {
    public:
        SkyShader();
        ~SkyShader();

        void Link() override;

        void InitUniforms(vd::object::EntityPtr pEntity) override;
        void UpdateUniforms(vd::object::EntityPtr pEntity, uint32_t meshIndex) override;
    private:
        void AddUniforms() override;

        vd::camera::CameraPtr m_pCamera;
        vd::window::WindowPtr m_pWindow;
        vd::property::GlobalPropertiesPtr m_pProperties;
    };
    typedef std::shared_ptr<SkyShader>	SkyShaderPtr;
}

#endif //VD_GAME_ENGINE_SKYSHADER_HPP
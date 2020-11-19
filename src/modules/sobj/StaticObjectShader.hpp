//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_STATICOBJECTSHADER_HPP
#define VD_GAME_ENGINE_STATICOBJECTSHADER_HPP

#include <engine/api/gl/Shader.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/property/GlobalProperties.hpp>
#include <engine/kernel/Context.hpp>
#include <engine/window/Window.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/light/LightManager.hpp>

namespace mod::sobj {
    class StaticObjectShader : public vd::gl::Shader, public vd::injector::Injectable {
    public:
        StaticObjectShader();
        ~StaticObjectShader();

        void Link() override;

        void InitUniforms(vd::object::EntityPtr pEntity) override;
        void UpdateUniforms(vd::object::EntityPtr pEntity, uint32_t meshIndex) override;
    private:
        void AddUniforms() override;

        vd::property::GlobalPropertiesPtr m_pProperties;
        vd::kernel::ContextPtr m_pContext;
        vd::camera::CameraPtr m_pCamera;
        vd::window::WindowPtr m_pWindow;
        vd::light::LightManagerPtr m_pLightManager;
    };
    typedef std::shared_ptr<StaticObjectShader>	StaticObjectShaderPtr;
}

#endif //VD_GAME_ENGINE_STATICOBJECTSHADER_HPP

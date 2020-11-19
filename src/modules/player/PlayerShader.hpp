//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_PLAYERSHADER_HPP
#define VD_GAME_ENGINE_PLAYERSHADER_HPP

#include <engine/api/gl/Shader.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include <engine/injector/ObjectOfType.hpp>
#include <engine/injector/Injectable.hpp>

#include <engine/window/Window.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/property/GlobalProperties.hpp>
#include <engine/light/LightManager.hpp>
#include <engine/kernel/Context.hpp>

namespace mod::player {
    class PlayerShader : public vd::gl::Shader, public vd::injector::Injectable {
    public:
        PlayerShader();
        ~PlayerShader();

        void Link() override;

        void InitUniforms(vd::object::EntityPtr pEntity) override;
        void UpdateUniforms(vd::object::EntityPtr pEntity, uint32_t meshIndex) override;
    private:
        void AddUniforms() override;

        vd::property::GlobalPropertiesPtr m_pProperties;
        vd::light::LightManagerPtr m_pLightManager;

        vd::window::WindowPtr m_pWindow;
        vd::camera::CameraPtr m_pCamera;

        vd::kernel::ContextPtr m_pContext;
    };
    typedef std::shared_ptr<PlayerShader>	PlayerShaderPtr;
}

#endif //VD_GAME_ENGINE_PLAYERSHADER_HPP
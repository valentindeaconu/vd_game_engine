//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_STATICOBJECTSHADER_HPP
#define VD_GAME_ENGINE_STATICOBJECTSHADER_HPP

#include <engine/component/IEntityShader.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/kernel/Context.hpp>
#include <engine/window/Window.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/light/LightManager.hpp>
#include <engine/fog/FogManager.hpp>

namespace mod::sobj {
    class StaticObjectShader
            : public vd::component::IEntityShader
            , public vd::injector::Injectable
            , public std::enable_shared_from_this<StaticObjectShader> {
    public:
        StaticObjectShader();
        ~StaticObjectShader();

        void Link() override;

        void InitUniforms(vd::object::EntityPtr pEntity) override;
        void UpdateUniforms(vd::object::EntityPtr pEntity, uint32_t meshIndex) override;
    private:
        void AddUniforms() override;

        vd::kernel::ContextPtr  m_pContext;
        vd::camera::CameraPtr   m_pCamera;
        vd::window::WindowPtr   m_pWindow;

        vd::light::LightManagerPtr  m_pLightManager;
        vd::fog::FogManagerPtr      m_pFogManager;
    };
    typedef std::shared_ptr<StaticObjectShader>	StaticObjectShaderPtr;
}

#endif //VD_GAME_ENGINE_STATICOBJECTSHADER_HPP

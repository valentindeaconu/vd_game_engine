//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_PROPSHADER_HPP
#define VD_GAME_ENGINE_PROPSHADER_HPP

#include <engine/component/IEntityShader.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/context/Context.hpp>
#include <engine/window/Window.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/light/LightManager.hpp>
#include <engine/fog/FogManager.hpp>

#include "Prop.hpp"

namespace mod::props {
    class PropShader
            : public vd::component::IEntity3DShader
            , public vd::injector::Injectable
            , public std::enable_shared_from_this<PropShader> {
    public:
        PropShader();
        ~PropShader();

        void Link() override;

        void InitUniforms(vd::object::Entity3DPtr pEntity) override;
        void UpdateUniforms(vd::object::Entity3DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) override;
    private:
        void AddUniforms() override;

        vd::context::ContextPtr  m_pContext;
        vd::camera::CameraPtr   m_pCamera;
        vd::window::WindowPtr   m_pWindow;

        vd::light::LightManagerPtr  m_pLightManager;
        vd::fog::FogManagerPtr      m_pFogManager;
    };
    typedef std::shared_ptr<PropShader> PropShaderPtr;
}

#endif //VD_GAME_ENGINE_PROPSHADER_HPP

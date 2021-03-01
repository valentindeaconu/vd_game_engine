//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_SKYSHADER_HPP
#define VD_GAME_ENGINE_SKYSHADER_HPP

#include <engine/component/IEntityShader.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include <engine/injector/Injectable.hpp>

#include <engine/camera/Camera.hpp>
#include <engine/window/Window.hpp>
#include <engine/fog/FogManager.hpp>

namespace mod::sky {
    class SkyShader
            : public vd::component::IEntity3DShader
            , public vd::injector::Injectable
            , public std::enable_shared_from_this<SkyShader> {
    public:
        SkyShader();
        ~SkyShader();

        void Link() override;

        void InitUniforms(vd::object::Entity3DPtr pEntity) override;
        void UpdateUniforms(vd::object::Entity3DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) override;
    private:
        void AddUniforms() override;

        vd::camera::CameraPtr   m_pCamera;
        vd::window::WindowPtr   m_pWindow;
        vd::fog::FogManagerPtr  m_pFogManager;
    };
    typedef std::shared_ptr<SkyShader>	SkyShaderPtr;
}

#endif //VD_GAME_ENGINE_SKYSHADER_HPP
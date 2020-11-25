//
// Created by Vali on 9/11/2020.
//

#ifndef VD_GAME_ENGINE_WATERSHADER_HPP
#define VD_GAME_ENGINE_WATERSHADER_HPP

#include <engine/api/gl/Shader.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/window/Window.hpp>
#include <engine/light/LightManager.hpp>

#include "Water.hpp"

#include <memory>

namespace mod::water {
    class WaterShader : public vd::gl::IEntityShader, public vd::injector::Injectable {
    public:
        WaterShader();
        ~WaterShader();

        void Link() override;

        void InitUniforms(vd::object::EntityPtr pEntity) override;
        void UpdateUniforms(vd::object::EntityPtr pEntity, uint32_t meshIndex) override;
    private:
        void AddUniforms() override;

        vd::camera::CameraPtr m_pCamera;
        vd::window::WindowPtr m_pWindow;
        vd::light::LightManagerPtr m_pLightManager;
    };
    typedef std::shared_ptr<WaterShader>    WaterShaderPtr;
}

#endif //VD_GAME_ENGINE_WATERSHADER_HPP

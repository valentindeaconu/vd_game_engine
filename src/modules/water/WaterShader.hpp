//
// Created by Vali on 9/11/2020.
//

#ifndef VD_GAME_ENGINE_WATERSHADER_HPP
#define VD_GAME_ENGINE_WATERSHADER_HPP

#include <engine/component/IEntityShader.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/window/Window.hpp>
#include <engine/light/LightManager.hpp>

#include "Water.hpp"

#include <memory>

namespace mod::water {
    class WaterShader
            : public vd::component::IEntity3DShader
            , public vd::injector::Injectable
            , protected std::enable_shared_from_this<WaterShader> {
    public:
        void Link() override;
        void Init() override;

        void InitUniforms(vd::object::Entity3DPtr pEntity) override;
        void UpdateUniforms(vd::object::Entity3DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) override;
    private:
        vd::camera::CameraPtr m_pCamera;
        vd::window::WindowPtr m_pWindow;
        vd::light::LightManagerPtr m_pLightManager;
    };
    typedef std::shared_ptr<WaterShader>    WaterShaderPtr;
}

#endif //VD_GAME_ENGINE_WATERSHADER_HPP

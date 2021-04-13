//
// Created by Vali on 4/6/2021.
//

#ifndef VDGE_SUNSHADER_HPP
#define VDGE_SUNSHADER_HPP

#include <memory>

#include <engine/object/Entity2D.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/window/Window.hpp>

#include <engine/component/IEntityShader.hpp>
#include <engine/loader/ShaderLoader.hpp>

namespace mod::sky {
    class SunShader : public vd::component::IEntity2DShader, public vd::injector::Injectable {
    public:
        void Link() override;
        void Init() override;

        void InitUniforms(vd::object::Entity2DPtr pEntity) override;
        void UpdateUniforms(vd::object::Entity2DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) override;
    private:
        vd::camera::CameraPtr m_pCamera;
        vd::window::WindowPtr m_pWindow;
    };
    typedef std::shared_ptr<SunShader>	SunShaderPtr;
}

#endif //VDGE_SUNSHADER_HPP

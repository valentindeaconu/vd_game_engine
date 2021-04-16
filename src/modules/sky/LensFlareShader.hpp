//
// Created by Vali on 4/6/2021.
//

#ifndef VDGE_LENSFLARESHADER_HPP
#define VDGE_LENSFLARESHADER_HPP

#include <memory>

#include <engine/gapi/gl/wrappers/Shader.hpp>
#include <engine/gapi/gl/Texture.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/window/Window.hpp>

namespace mod::sky {

    class FlareShader : public vd::gl::wrappers::Shader, public vd::injector::Injectable {
    public:
        void Link() override;

        void Init();
        void UpdateUniforms(const vd::gl::Texture2DPtr& texture, const glm::vec4& transform, float brightness);
    private:
        vd::camera::CameraPtr m_pCamera;
        vd::window::WindowPtr m_pWindow;
    };
    typedef std::shared_ptr<FlareShader>	FlareShaderPtr;

}

#endif //VDGE_LENSFLARESHADER_HPP
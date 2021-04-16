//
// Created by Vali on 4/3/2021.
//

#ifndef VDGE_PARTICLESHADER_HPP
#define VDGE_PARTICLESHADER_HPP

#include <engine/gapi/gl/wrappers/Shader.hpp>

#include <engine/loader/ShaderLoader.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/window/Window.hpp>

#include "ParticleSystem.hpp"

namespace mod::particles {
    
    class ParticleShader : public vd::gl::wrappers::Shader, public vd::injector::Injectable {
    public:
        void Link() override;

        void Init();
        void UpdateUniforms(const ParticleSystemPtr& pParticleSystem);
    private:
        vd::camera::CameraPtr m_pCamera;
        vd::window::WindowPtr m_pWindow;
    };
    typedef std::shared_ptr<ParticleShader>	ParticleShaderPtr;


}

#endif //VDGE_PARTICLESHADER_HPP
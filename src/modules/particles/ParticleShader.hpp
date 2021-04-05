//
// Created by Vali on 4/3/2021.
//

#ifndef VDGE_PARTICLESHADER_HPP
#define VDGE_PARTICLESHADER_HPP

#include <engine/component/IEntityShader.hpp>

#include <engine/loader/ShaderLoader.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/window/Window.hpp>

namespace mod::particles {
    
    class ParticleShader : public vd::component::IEntityShader, public vd::injector::Injectable {
    public:
        void Link() override;

        void Init() override;

        void InitUniforms(vd::object::EntityPtr pEntity);
        void UpdateUniforms(vd::object::EntityPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex);
    private:
        vd::camera::CameraPtr m_pCamera;
        vd::window::WindowPtr m_pWindow;
    };
    typedef std::shared_ptr<ParticleShader>	ParticleShaderPtr;


}

#endif //VDGE_PARTICLESHADER_HPP
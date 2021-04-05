//
// Created by Vali on 4/3/2021.
//

#include "ParticleShader.hpp"

namespace mod::particles {

    void ParticleShader::Link() {
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
    }

    void ParticleShader::Init() {
        Create();

        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/particle/particle_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string gSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/particle/particle_GS.glsl", gSource);
        AddShader(gSource, vd::gl::Shader::eGeometryShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/particle/particle_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();

        AddUniform("uView");
        AddUniform("uProjection");

        AddUniform("uCameraUp");
        AddUniform("uCameraRight");

        AddUniform("uAtlasSampler.Atlas");
        AddUniform("uAtlasSampler.Size");
    }
    
    void ParticleShader::InitUniforms(vd::object::EntityPtr pEntity) {

    }

    void ParticleShader::UpdateUniforms(vd::object::EntityPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) {
        SetUniform("uView", m_pCamera->ViewMatrix());
        SetUniform("uProjection", m_pWindow->ProjectionMatrix());

        SetUniform("uCameraUp", m_pCamera->Up());
        SetUniform("uCameraRight", m_pCamera->Right());
    }


}
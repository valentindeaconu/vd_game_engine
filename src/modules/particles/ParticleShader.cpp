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

        AddUniform("uProjection");

        AddUniform("uAtlasSampler.Atlas");
        AddUniform("uAtlasSampler.Size");
    }

    void ParticleShader::UpdateUniforms(const ParticleSystemPtr& pParticleSystem) {
        SetUniform("uProjection", m_pWindow->ProjectionMatrix());

        pParticleSystem->TextureAtlas()->BindToUnit(0);
        SetUniform("uAtlasSampler.Atlas", 0);
        SetUniform("uAtlasSampler.Size", int(pParticleSystem->TextureAtlas()->Size()));
    }


}
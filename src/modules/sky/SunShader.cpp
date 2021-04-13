//
// Created by Vali on 4/6/2021.
//

#include "SunShader.hpp"

namespace mod::sky {
    void SunShader::Link() {
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
    }

    void SunShader::Init() {
        Create();

        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/sun/sun_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string gSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/sun/sun_GS.glsl", gSource);
        AddShader(gSource, vd::gl::Shader::eGeometryShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/sun/sun_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();

        AddUniform("model");
        AddUniform("view");
        AddUniform("projection");

        AddUniform("diffuseMap");

        AddUniform("scale");
        AddUniform("cameraUp");
        AddUniform("cameraRight");
    }

    void SunShader::InitUniforms(vd::object::Entity2DPtr pEntity) {

    }

    void SunShader::UpdateUniforms(vd::object::Entity2DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) {
        SetUniform("model", pEntity->WorldTransform().Get());
        SetUniform("view", m_pCamera->ViewMatrix());
        SetUniform("projection", m_pWindow->ProjectionMatrix());

        SetUniform("cameraUp", m_pCamera->Up());
        SetUniform("cameraRight", m_pCamera->Right());

        const vd::model::Mesh2DPtr& pMesh = pEntity->Meshes()[meshIndex];

        SetUniform("scale", pEntity->LocalTransform().Scale().x);

        auto& diffuseMap = pMesh->Materials()[0].DiffuseMap();

        diffuseMap->BindToUnit(0);
        SetUniform("diffuseMap", 0);
    }
}
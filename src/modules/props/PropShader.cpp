#include "PropShader.hpp"

namespace mod::props {

    PropShader::PropShader() : vd::component::IEntityShader() {
        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/entity/entity_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/entity/entity_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
    }

    PropShader::~PropShader() = default;

    void PropShader::Link() {
        m_pContext = vd::ObjectOfType<vd::kernel::Context>::Find();
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();

        m_pLightManager = vd::ObjectOfType<vd::light::LightManager>::Find();
        m_pFogManager = vd::ObjectOfType<vd::fog::FogManager>::Find();
    }

    void PropShader::AddUniforms() {
        AddUniform("model");
        AddUniform("view");
        AddUniform("projection");

        AddUniform("diffuseMap");
        AddUniform("specularMap");

        m_pFogManager->AddUniforms(shared_from_this());
        m_pLightManager->AddUniforms(shared_from_this());

        AddUniform("clipPlane");
    }

    void PropShader::InitUniforms(vd::object::EntityPtr pEntity) {
        AddUniforms();

        m_pFogManager->SetUniforms(shared_from_this());
        m_pLightManager->SetUniforms(shared_from_this());
    }


    void PropShader::UpdateUniforms(vd::object::EntityPtr pEntity, uint32_t meshIndex) {
        SetUniform("model", pEntity->WorldTransform().Get());

        SetUniform("view", m_pCamera->ViewMatrix());
        SetUniform("projection", m_pWindow->ProjectionMatrix());

        vd::model::MeshPtr& meshPtr = pEntity->Meshes()[meshIndex];

        if (!meshPtr->materials.empty()) {
            vd::model::Material& meshMaterial = meshPtr->materials.front();

            if (meshMaterial.diffuseMap != nullptr) {
                vd::gl::ActiveTexture(0);
                meshMaterial.diffuseMap->Bind();
                SetUniform("diffuseMap", 0);
            }

            if (meshMaterial.specularMap != nullptr) {
                vd::gl::ActiveTexture(1);
                meshMaterial.specularMap->Bind();
                SetUniform("specularMap", 1);
            }
        }

        SetUniform("clipPlane", m_pContext->ClipPlane());
    }
}
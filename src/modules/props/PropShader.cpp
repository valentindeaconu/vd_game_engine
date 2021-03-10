#include "PropShader.hpp"

namespace mod::props {

    PropShader::PropShader() : vd::component::IEntity3DShader() {
        Create();

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
        m_pContext = vd::ObjectOfType<vd::context::Context>::Find();
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

        AddUniform("fakeLighting");
    }

    void PropShader::InitUniforms(vd::object::Entity3DPtr pEntity) {
        AddUniforms();

        m_pFogManager->SetUniforms(shared_from_this());
        m_pLightManager->SetUniforms(shared_from_this());
    }


    void PropShader::UpdateUniforms(vd::object::Entity3DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) {
        SetUniform("model", pEntity->WorldTransform().Get());

        SetUniform("view", m_pCamera->ViewMatrix());
        SetUniform("projection", m_pWindow->ProjectionMatrix());

        const vd::model::Mesh3DPtr& pMesh = pEntity->Meshes(levelOfDetail)[meshIndex];

        if (!pMesh->Materials().empty()) {
            vd::model::Material& meshMaterial = pMesh->Materials().front();

            if (meshMaterial.DiffuseMap() != nullptr) {
                vd::gl::ActiveTexture(0);
                meshMaterial.DiffuseMap()->Bind();
                SetUniform("diffuseMap", 0);
            }

            if (meshMaterial.SpecularMap() != nullptr) {
                vd::gl::ActiveTexture(1);
                meshMaterial.SpecularMap()->Bind();
                SetUniform("specularMap", 1);
            }
        }

        SetUniform("clipPlane", m_pContext->ClipPlane());

        auto pProp = std::dynamic_pointer_cast<Prop>(pEntity);

        if (pProp->BillboardAtLevel(levelOfDetail)) {
            SetUniform("fakeLighting", 1);
        } else {
            SetUniform("fakeLighting", 0);
        }
    }
}
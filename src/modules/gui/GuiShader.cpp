//
// Created by Vali on 8/29/2020.
//

#include "GuiShader.hpp"

namespace mod::gui {

    void GuiShader::Init() {
        if (!m_Initialized) {
            Create();

            std::string vsSource;
            vd::loader::ShaderLoader::Load("./resources/shaders/gui/gui_VS.glsl", vsSource);
            AddShader(vsSource, vd::gl::Shader::eVertexShader);

            std::string fsSource;
            vd::loader::ShaderLoader::Load("./resources/shaders/gui/gui_FS.glsl", fsSource);
            AddShader(fsSource, vd::gl::Shader::eFragmentShader);

            Compile();

            AddUniform("transform");
            AddUniform("diffuseMap");
            
            m_Initialized = true;
        }
    }

    void GuiShader::InitUniforms(vd::object::Entity2DPtr pEntity) {
        
    }

    void GuiShader::UpdateUniforms(vd::object::Entity2DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) {
        SetUniform("transform", pEntity->LocalTransform().Get());

        pEntity->Meshes()[meshIndex]->Materials()[0].DiffuseMap()->BindToUnit(1);
        SetUniform("diffuseMap", 1);
    }
}

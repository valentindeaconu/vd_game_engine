//
// Created by Vali on 2/18/2021.
//

#include "GuiTextShader.hpp"

namespace mod::gui {

    void GuiTextShader::Init() {
        if (!m_Initialized) {
            Create();

            std::string vsSource;
            vd::loader::ShaderLoader::Load("./resources/shaders/gui/text_VS.glsl", vsSource);
            AddShader(vsSource, vd::gl::Shader::eVertexShader);

            std::string fsSource;
            vd::loader::ShaderLoader::Load("./resources/shaders/gui/text_FS.glsl", fsSource);
            AddShader(fsSource, vd::gl::Shader::eFragmentShader);

            Compile();

            AddUniform("projection");
            AddUniform("text");
            AddUniform("textColor");
            
            m_Initialized = true;
        }
    }

    void GuiTextShader::Link() {
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
    }

    void GuiTextShader::InitUniforms(vd::object::Entity2DPtr pEntity) {
        SetUniform("projection", m_pWindow->OrthoProjectionMatrix());
    }

    void GuiTextShader::UpdateUniforms(vd::object::Entity2DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) {
        mod::gui::GuiTextPtr pGuiText = std::dynamic_pointer_cast<mod::gui::GuiText>(pEntity);

        SetUniform("textColor", glm::vec3(pEntity->Meshes()[0]->Materials()[0].Color()));

        pGuiText->Font()->Characters()[pGuiText->Text()[meshIndex]].Texture->BindToUnit(1);
        SetUniform("text", 1);
    }
}

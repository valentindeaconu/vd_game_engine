//
// Created by Vali on 2/18/2021.
//

#include "GuiTextShader.hpp"

namespace mod::gui {

    GuiTextShader::GuiTextShader() : vd::component::IEntity2DShader() {
        Create();

        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/gui/text_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/gui/text_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
    }

    void GuiTextShader::Link() {
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
    }

    void GuiTextShader::AddUniforms() {
        AddUniform("projection");
        AddUniform("text");
        AddUniform("textColor");
    }

    void GuiTextShader::InitUniforms(vd::object::Entity2DPtr pEntity) {
        AddUniforms();

        SetUniform("projection", m_pWindow->OrthoProjectionMatrix());
    }

    void GuiTextShader::UpdateUniforms(vd::object::Entity2DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) {
        mod::gui::GuiTextPtr pGuiText = std::dynamic_pointer_cast<mod::gui::GuiText>(pEntity);

        SetUniform("textColor", glm::vec3(pEntity->Meshes()[0]->Materials()[0].Color()));

        vd::gl::ActiveTexture(1);
        pGuiText->Font()->Characters()[pGuiText->Text()[meshIndex]].Texture->Bind();
        SetUniform("text", 1);
    }
}

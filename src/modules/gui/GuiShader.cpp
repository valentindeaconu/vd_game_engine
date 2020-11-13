//
// Created by Vali on 8/29/2020.
//

#include "GuiShader.hpp"

namespace mod::gui {

    GuiShader::GuiShader() : vd::shader::Shader() {
        loadAndAddShader("./resources/shaders/gui/gui_VS.glsl", vd::shader::eVertexShader);
        loadAndAddShader("./resources/shaders/gui/gui_FS.glsl", vd::shader::eFragmentShader);

        compileShader();

        addUniform("transform");

        addUniform("guiTexture");
    }

    GuiShader::~GuiShader() = default;

    void GuiShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) {
        setUniform("transform", entityPtr->LocalTransform().Get());

        vd::gl::ActiveTexture(1);
        entityPtr->Meshes()[0]->materials[0].diffuseMap->Bind();
        setUniformi("guiTexture", 1);
    }
}

//
// Created by Vali on 2/19/2021.
//

#include "ContrastShader.hpp"

namespace mod::postprocessing {

    ContrastShader::ContrastShader() {
        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/pp_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/contrast_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
    }

    void ContrastShader::AddUniforms() {
        AddUniform("colorMap");
    }

    void ContrastShader::InitUniforms(vd::postprocessing::StagePtr pStage) {
        AddUniforms();
    }

    void ContrastShader::UpdateUniforms(vd::postprocessing::StagePtr pStage) {
        auto pContrast = std::dynamic_pointer_cast<vd::postprocessing::SingularInputStage>(pStage);

        vd::gl::ActiveTexture(0);
        pContrast->InputFrameBuffer()->ColorTexture()->Bind();
        SetUniform("colorMap", 0);
    }


}


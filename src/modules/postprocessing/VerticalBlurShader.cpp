//
// Created by Vali on 2/24/2021.
//

#include "VerticalBlurShader.hpp"

namespace mod::postprocessing {

    VerticalBlurShader::VerticalBlurShader() {
        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/verticalBlur_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/blur_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
    }

    void VerticalBlurShader::AddUniforms() {
        AddUniform("colorMap");
        AddUniform("targetWidth");
    }

    void VerticalBlurShader::InitUniforms(vd::postprocessing::StagePtr pStage) {
        AddUniforms();
    }

    void VerticalBlurShader::UpdateUniforms(vd::postprocessing::StagePtr pStage) {
        auto pVBlur = std::dynamic_pointer_cast<vd::postprocessing::SingularInputStage>(pStage);

        vd::gl::ActiveTexture(0);
        pVBlur->InputFrameBuffer()->ColorTexture()->Bind();
        SetUniform("colorMap", 0);

        auto targetWidth = float(pVBlur->FrameBuffer()->Dimension().width);
        SetUniform("targetWidth", targetWidth);
    }

}
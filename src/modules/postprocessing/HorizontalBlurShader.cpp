//
// Created by Vali on 2/24/2021.
//

#include "HorizontalBlurShader.hpp"

namespace mod::postprocessing {


    HorizontalBlurShader::HorizontalBlurShader() {
        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/horizontalBlur_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/blur_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
    }

    void HorizontalBlurShader::AddUniforms() {
        AddUniform("colorMap");
        AddUniform("targetWidth");
    }

    void HorizontalBlurShader::InitUniforms(vd::postprocessing::StagePtr pStage) {
        AddUniforms();
    }

    void HorizontalBlurShader::UpdateUniforms(vd::postprocessing::StagePtr pStage) {
        auto pHBlur = std::dynamic_pointer_cast<vd::postprocessing::SingularInputStage>(pStage);

        vd::gl::ActiveTexture(0);
        pHBlur->InputFrameBuffer()->ColorTexture()->Bind();
        SetUniform("colorMap", 0);

        auto targetWidth = float(pHBlur->FrameBuffer()->Dimension().width);
        SetUniform("targetWidth", targetWidth);
    }
}
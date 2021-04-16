//
// Created by Vali on 2/24/2021.
//

#include "VerticalBlurShader.hpp"

namespace mod::postprocessing {

    VerticalBlurShader::VerticalBlurShader() {
        Create();

        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/verticalBlur_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::wrappers::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/blur_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::wrappers::Shader::eFragmentShader);

        Compile();
    }

    void VerticalBlurShader::AddUniforms() {
        AddUniform("colorMap");
        AddUniform("targetWidth");
    }

    void VerticalBlurShader::InitUniforms(vd::component::IRenderingEffectPtr pEffect) {
        AddUniforms();
    }

    void VerticalBlurShader::UpdateUniforms(vd::component::IRenderingEffectPtr pEffect) {
        auto pVBlur = std::dynamic_pointer_cast<vd::component::ConcreteEffect>(pEffect);

        pVBlur->InputFrameBuffer()->ColorTexture()->BindToUnit(0);
        SetUniform("colorMap", 0);

        auto targetWidth = float(pVBlur->FrameBuffer()->Dimension().width);
        SetUniform("targetWidth", targetWidth);
    }

}
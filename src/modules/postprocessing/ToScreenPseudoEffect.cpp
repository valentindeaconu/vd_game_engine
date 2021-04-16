//
// Created by Vali on 2/26/2021.
//

#include "ToScreenPseudoEffect.hpp"

namespace mod::postprocessing {

    ToScreenPseudoEffect::ToScreenPseudoEffect(vd::component::FrameBuffersResolver inputGetter)
        : vd::component::ConcreteEffect("ToScreen",
                                        1.0f,
                                        std::move(inputGetter),
                                        [](vd::gl::FrameBufferPtr&) { },
                                        vd::g_kEmptyPredicate,
                                        []() { glDisable(GL_DEPTH_TEST); },
                                        []() { glEnable(GL_DEPTH_TEST); })
    {
    }

    void ToScreenPseudoEffect::Init(vd::Dimension dimension) {
        ConcreteEffect::Init(dimension);

        FrameBuffer() = std::make_shared<vd::gl::FrameBuffer>(dimension.width, dimension.height);
    }

    ToScreenShader::ToScreenShader() {
        Create();

        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/pp_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::wrappers::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/to_screen_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::wrappers::Shader::eFragmentShader);

        Compile();
    }

    void ToScreenShader::Link() {
        m_pContext = vd::ObjectOfType<vd::context::Context>::Find();
    }

    void ToScreenShader::AddUniforms() {
        AddUniform("colorMap");
    }

    void ToScreenShader::InitUniforms(vd::component::IRenderingEffectPtr pEffect) {
        AddUniforms();
    }

    void ToScreenShader::UpdateUniforms(vd::component::IRenderingEffectPtr pEffect) {
         auto pToScreenPseudoEffect = std::dynamic_pointer_cast<ToScreenPseudoEffect>(pEffect);

        if (m_pContext->WireframeMode()) {
            m_pContext->SceneFrameBuffer()->ColorTexture()->BindToUnit(0);
        } else {
            pToScreenPseudoEffect->InputFrameBuffer()->ColorTexture()->BindToUnit(0);
        }
        SetUniform("colorMap", 0);
    }
}
//
// Created by Vali on 2/19/2021.
//

#include "PostProcessing.hpp"

namespace vd::postprocessing {

    void PostProcessing::Link() {
        m_pContext = vd::ObjectOfType<kernel::Context>::Find();
        m_pWindow = vd::ObjectOfType<window::Window>::Find();
    }

    void PostProcessing::Init() {
        // Create to screen shader
        m_pToScreenShader = std::make_shared<gl::Shader>();

        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/pp_VS.glsl", vsSource);
        m_pToScreenShader->AddShader(vsSource, vd::gl::Shader::eVertexShader);
        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/to_screen_FS.glsl", fsSource);
        m_pToScreenShader->AddShader(fsSource, vd::gl::Shader::eFragmentShader);
        m_pToScreenShader->Compile();
        m_pToScreenShader->AddUniform("colorMap");

        // Create screen quad
        glm::vec2 vertices[] = {
                glm::vec2(-1.0f, 1.0f),
                glm::vec2(-1.0f, -1.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(1.0f, -1.0f)
        };
        GLuint indices[] = { 0, 1, 2, 2, 1, 3 };

        m_pQuadBuffer = std::make_shared<gl::Buffer>();
        m_pQuadBuffer->Bind();
        m_pQuadBuffer->AddBuffer(gl::buffer::eArrayBuffer,
                                 4 * sizeof(glm::vec2),
                                 vertices,
                                 gl::buffer::eStaticDraw);
        m_pQuadBuffer->AddBuffer(gl::buffer::eElementArrayBuffer,
                                 6 * sizeof(GLuint),
                                 indices,
                                 gl::buffer::eStaticDraw);
        m_pQuadBuffer->AttributeArray(0, 2, vd::gl::eFloat, sizeof(glm::vec2), (GLvoid*)0);
        m_pQuadBuffer->Unbind();

        for (auto& s : m_Stages) {
            s.Stage->Init();
            s.Shader->Bind();
            s.Shader->InitUniforms(s.Stage);
            s.Shader->Unbind();
        }
    }

    void PostProcessing::Render() {
        glFrontFace(GL_CCW);

        gl::Texture2DPtr pToScreenTex = nullptr;

        for (auto& s : m_Stages) {
            StagePtr &stage = s.Stage;
            IStageShaderPtr& shader = s.Shader;

            if (stage->Precondition()) {
                stage->Prepare();
                stage->FrameBuffer()->Bind();

                shader->Bind();
                shader->UpdateUniforms(stage);

                glClear(GL_COLOR_BUFFER_BIT);
                m_pQuadBuffer->DrawElements(vd::gl::eTriangles, 6, vd::gl::eUnsignedInt);

                shader->Unbind();

                stage->FrameBuffer()->Unbind();
                stage->Finish();

                pToScreenTex = stage->FrameBuffer()->ColorTexture();
            }
        }

        if (m_Stages.empty()) {
            pToScreenTex = m_pContext->SceneFBO()->ColorTexture();
        }

        m_pToScreenShader->Bind();

        gl::ActiveTexture(0);
        pToScreenTex->Bind();
        m_pToScreenShader->SetUniform("colorMap", 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, m_pWindow->Width(), m_pWindow->Height());
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        m_pQuadBuffer->DrawElements(vd::gl::eTriangles, 6, vd::gl::eUnsignedInt);

        m_pToScreenShader->Unbind();

        glFrontFace(GL_CW);
    }

    void PostProcessing::CleanUp() {
        for (auto& s : m_Stages) {
            s.Stage->CleanUp();
        }
    }

    void PostProcessing::PushStage(const StagePtr& stage, const IStageShaderPtr& shader) {
        StageShader s;
        s.Stage = stage;
        s.Shader = shader;

        m_Stages.push_back(s);
    }

    void PostProcessing::RemoveStage(const std::string& name) {
        auto it = std::find_if(m_Stages.begin(), m_Stages.end(), [&](auto& s) {
            return s.Stage->Name() == name;
        });

        if (it != m_Stages.end()) {
            m_Stages.erase(it);
        }
    }

}
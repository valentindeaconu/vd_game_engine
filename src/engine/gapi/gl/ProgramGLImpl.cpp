//
// Created by vali on 4/16/21.
//

#include "ProgramGLImpl.hpp"

namespace vd::gl {

    void ProgramGLImpl::Create(const std::vector<std::pair<gapi::GraphicPipelineStage, std::string>>& programSources) {
        if (m_Shader) {
            m_Shader->CleanUp();
            m_Shader = nullptr;
        }
        m_Shader = std::make_unique<wrappers::Shader>();

        m_Shader->Create();

        for (auto& [stage, source] : programSources) {
            m_Shader->AddShader(source, ToShaderType(stage));
        }

        m_Shader->Compile();
    }

    void ProgramGLImpl::Use() {
        m_Shader->Bind();
    }

    void ProgramGLImpl::Discard() {
        m_Shader->Unbind();
    }

    void ProgramGLImpl::SetEmpty(const std::string& name) {
        m_Shader->AddUniform(name);
    }

    void ProgramGLImpl::Set(const std::string& name, int value) {
        m_Shader->SetUniform(name, value);
    }

    void ProgramGLImpl::Set(const std::string& name, float value) {
        m_Shader->SetUniform(name, value);
    }

    void ProgramGLImpl::Set(const std::string& name, const glm::vec2& value) {
        m_Shader->SetUniform(name, value);
    }

    void ProgramGLImpl::Set(const std::string& name, const glm::vec3& value) {
        m_Shader->SetUniform(name, value);
    }

    void ProgramGLImpl::Set(const std::string& name, const glm::vec4& value) {
        m_Shader->SetUniform(name, value);
    }

    void ProgramGLImpl::Set(const std::string& name, const glm::quat& value) {
        m_Shader->SetUniform(name, value);
    }

    void ProgramGLImpl::Set(const std::string& name, const glm::mat3& value) {
        m_Shader->SetUniform(name, value);
    }

    void ProgramGLImpl::Set(const std::string& name, const glm::mat4& value) {
        m_Shader->SetUniform(name, value);
    }

    void ProgramGLImpl::Release() {
        m_Shader->CleanUp();
    }

    wrappers::Shader::Type ProgramGLImpl::ToShaderType(const gapi::GraphicPipelineStage& stage) {
        switch (stage) {
            case gapi::GraphicPipelineStage::eVertex:                   return wrappers::Shader::eVertexShader;
            case gapi::GraphicPipelineStage::eTessellationControl:      return wrappers::Shader::eTessellationControlShader;
            case gapi::GraphicPipelineStage::eTessellationEvaluation:   return wrappers::Shader::eTessellationEvaluationShader;
            case gapi::GraphicPipelineStage::eGeometry:                 return wrappers::Shader::eGeometryShader;
            case gapi::GraphicPipelineStage::eFragment:                 return wrappers::Shader::eFragmentShader;
            case gapi::GraphicPipelineStage::eCompute:                  return wrappers::Shader::eComputeShader;
        }

        return wrappers::Shader::eVertexShader;
    }


}
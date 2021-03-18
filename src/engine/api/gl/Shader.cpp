//
// Created by Vali on 11/18/2020.
//

#include "Shader.hpp"

namespace vd::gl {

    namespace exception {
        ShaderError::ShaderError(const std::string &stage, const std::string &message)
                : CustomException("ShaderError", "[" + stage + "]: " + message) {

        }


        UniformException::UniformException(const std::string &message, const std::string &uniformName)
                : CustomException("UniformException", message + ": " + uniformName) {
        }


        UniformLocationException::UniformLocationException(const std::string &uniformName)
                : UniformException("Could not locate uniform", uniformName) {
        }

        UniformNotLocatedException::UniformNotLocatedException(const std::string &uniformName)
                : UniformException("Uniform is not located", uniformName) {

        }
    }

    std::string Shader::ToString(const Shader::Type &type) {
        switch (type) {
            case eVertexShader:
                return "Vertex Shader";
            case eTessellationControlShader:
                return "Tessellation Control Shader";
            case eTessellationEvaluationShader:
                return "Tessellation Evaluation Shader";
            case eGeometryShader:
                return "Geometry Shader";
            case eComputeShader:
                return "Compute Shader";
            case eFragmentShader:
                return "Fragment Shader";
            default:
                return "Unknown";
        }
    }

    Shader::Shader()
        : m_Program(0)
        , m_Bound(false)
        , m_UniformMap()
    {
    }

    void Shader::OnCreate() {
        m_Program = glCreateProgram();
    }

    void Shader::OnCleanUp() {
        glDeleteProgram(m_Program);
    }

    void Shader::Bind() {
        PassIfCreated();

        if (!m_Bound) {
            glUseProgram(m_Program);
            m_Bound = true;
        }
    }

    void Shader::Unbind() {
        if (m_Bound) {
            glUseProgram(0);
            m_Bound = false;
        }
    }

    void Shader::AddShader(const std::string& source, const Shader::Type& type) {
        PassIfCreated();

        uint32_t shader = glCreateShader(type);

        if (shader == 0) {
            throw exception::ShaderError("Shader Creation @" + ToString(type), "Could not create a new shader");
        }

        const char *shaderSource = source.c_str();
        glShaderSource(shader, 1, &shaderSource, nullptr);

        glCompileShader(shader);

        int compileStatus;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

        if (!compileStatus) {
            char infoLog[1024];
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);

            glDeleteShader(shader);
            throw exception::ShaderError("Shader Compilation @" + ToString(type), infoLog);
        }

        if (m_Program == 0) {
            glDeleteShader(shader);
            throw exception::ShaderError("Shader Attach @" + ToString(type), "There's no program to attach to");
        }

        glAttachShader(m_Program, shader);
        glDeleteShader(shader);
    }

    void Shader::Compile() {
        PassIfCreated();

        glLinkProgram(m_Program);

        int linkStatus;
        glGetProgramiv(m_Program, GL_LINK_STATUS, &linkStatus);

        if (!linkStatus) {
            char infoLog[1024];
            glGetProgramInfoLog(m_Program, 1024, nullptr, infoLog);

            throw exception::ShaderError("Linking", infoLog);
        }

        int validateStatus;
        glGetProgramiv(m_Program, GL_VALIDATE_STATUS, &validateStatus);

        if (!validateStatus) {
            char infoLog[1024];
            glGetProgramInfoLog(m_Program, 1024, nullptr, infoLog);

            throw exception::ShaderError("Validation", infoLog);
        }
    }

    uint32_t Shader::Program() const {
        return m_Program;
    }

    void Shader::AddUniform(const std::string& uniformName) {
        PassIfCreated();

        uint32_t location = glGetUniformLocation(m_Program, uniformName.c_str());

        if (location == 0xFFFFFFFF) {
            throw exception::UniformLocationException(uniformName);
        }

        m_UniformMap[uniformName] = location;
    }

    void Shader::AddUniformBlock(const std::string& uniformBlockName) {
        PassIfCreated();

        uint32_t location = glGetUniformBlockIndex(m_Program, uniformBlockName.c_str());

        if (location == 0xFFFFFFFF) {
            throw exception::UniformLocationException(uniformBlockName);
        }

        m_UniformMap[uniformBlockName] = location;
    }

    void Shader::SetUniform(const std::string& uniformName, int value) const {
        PassIfCreated();

        auto it = m_UniformMap.find(uniformName);
        if (it != m_UniformMap.end()) {
            glUniform1i(it->second, value);
        } else {
            throw exception::UniformNotLocatedException(uniformName);
        }
    }

    void Shader::SetUniform(const std::string& uniformName, float value) const {
        PassIfCreated();

        auto it = m_UniformMap.find(uniformName);
        if (it != m_UniformMap.end()) {
            glUniform1f(it->second, value);
        } else {
            throw exception::UniformNotLocatedException(uniformName);
        }
    }

    void Shader::SetUniform(const std::string& uniformName, const glm::vec2& value) const {
        PassIfCreated();

        auto it = m_UniformMap.find(uniformName);
        if (it != m_UniformMap.end()) {
            glUniform2f(it->second, value.x, value.y);
        } else {
            throw exception::UniformNotLocatedException(uniformName);
        }
    }

    void Shader::SetUniform(const std::string& uniformName, const glm::vec3& value) const {
        PassIfCreated();

        auto it = m_UniformMap.find(uniformName);
        if (it != m_UniformMap.end()) {
            glUniform3f(it->second, value.x, value.y, value.z);
        } else {
            throw exception::UniformNotLocatedException(uniformName);
        }
    }

    void Shader::SetUniform(const std::string& uniformName, const glm::vec4& value) const {
        PassIfCreated();

        auto it = m_UniformMap.find(uniformName);
        if (it != m_UniformMap.end()) {
            glUniform4f(it->second, value.x, value.y, value.z, value.w);
        } else {
            throw exception::UniformNotLocatedException(uniformName);
        }
    }

    void Shader::SetUniform(const std::string& uniformName, const glm::quat& value) const {
        PassIfCreated();

        auto it = m_UniformMap.find(uniformName);
        if (it != m_UniformMap.end()) {
            glUniform4f(it->second, value.x, value.y, value.z, value.w);
        } else {
            throw exception::UniformNotLocatedException(uniformName);
        }
    }

    void Shader::SetUniform(const std::string& uniformName, const glm::mat3& value) const {
        PassIfCreated();
        
        auto it = m_UniformMap.find(uniformName);
        if (it != m_UniformMap.end()) {
            glUniformMatrix3fv(it->second, 1, false, glm::value_ptr(value));
        } else {
            throw exception::UniformNotLocatedException(uniformName);
        }
    }

    void Shader::SetUniform(const std::string& uniformName, const glm::mat4& value) const {
        PassIfCreated();

        auto it = m_UniformMap.find(uniformName);
        if (it != m_UniformMap.end()) {
            glUniformMatrix4fv(it->second, 1, false, glm::value_ptr(value));
        } else {
            throw exception::UniformNotLocatedException(uniformName);
        }
    }

    void Shader::PushUniform(const std::string& uniformName, int value) {
        PassIfCreated();

        auto it = m_UniformMap.find(uniformName);
        if (it == m_UniformMap.end()) {
            AddUniform(uniformName);
        }

        glUniform1i(m_UniformMap[uniformName], value);
    }

    void Shader::PushUniform(const std::string& uniformName, float value) {
        PassIfCreated();

        auto it = m_UniformMap.find(uniformName);
        if (it == m_UniformMap.end()) {
            AddUniform(uniformName);
        }

        glUniform1f(m_UniformMap[uniformName], value);
    }

    void Shader::PushUniform(const std::string& uniformName, const glm::vec2& value) {
        PassIfCreated();

        auto it = m_UniformMap.find(uniformName);
        if (it == m_UniformMap.end()) {
            AddUniform(uniformName);
        }

        glUniform2f(m_UniformMap[uniformName], value.x, value.y);
    }

    void Shader::PushUniform(const std::string& uniformName, const glm::vec3& value) {
        PassIfCreated();

        auto it = m_UniformMap.find(uniformName);
        if (it == m_UniformMap.end()) {
            AddUniform(uniformName);
        }

        glUniform3f(m_UniformMap[uniformName], value.x, value.y, value.z);
    }

    void Shader::PushUniform(const std::string& uniformName, const glm::vec4& value) {
        PassIfCreated();

        auto it = m_UniformMap.find(uniformName);
        if (it == m_UniformMap.end()) {
            AddUniform(uniformName);
        }

        glUniform4f(m_UniformMap[uniformName], value.x, value.y, value.z, value.w);
    }

    void Shader::PushUniform(const std::string& uniformName, const glm::quat& value) {
        PassIfCreated();

        auto it = m_UniformMap.find(uniformName);
        if (it == m_UniformMap.end()) {
            AddUniform(uniformName);
        }

        glUniform4f(m_UniformMap[uniformName], value.x, value.y, value.z, value.w);
    }

    void Shader::PushUniform(const std::string& uniformName, const glm::mat3& value) {
        PassIfCreated();
        
        auto it = m_UniformMap.find(uniformName);
        if (it == m_UniformMap.end()) {
            AddUniform(uniformName);
        }

        glUniformMatrix3fv(m_UniformMap[uniformName], 1, false, glm::value_ptr(value));
    }

    void Shader::PushUniform(const std::string& uniformName, const glm::mat4& value) {
        PassIfCreated();

        auto it = m_UniformMap.find(uniformName);
        if (it == m_UniformMap.end()) {
            AddUniform(uniformName);
        }

        glUniformMatrix4fv(m_UniformMap[uniformName], 1, false, glm::value_ptr(value));
    }

    void Shader::BindUniformBlock(const std::string& uniformBlockName, uint32_t uniformBlockBinding) const {
        PassIfCreated();

        auto it = m_UniformMap.find(uniformBlockName);
        if (it != m_UniformMap.end()) {
            glUniformBlockBinding(m_Program, it->second, uniformBlockBinding);
        } else {
            throw exception::UniformException("Could not locate uniform block", uniformBlockName);
        }
    }

    void Shader::BindFragDataLocation(const std::string& name, uint32_t index) const {
        PassIfCreated();
        
        glBindFragDataLocation(m_Program, index, name.c_str());
    }
}
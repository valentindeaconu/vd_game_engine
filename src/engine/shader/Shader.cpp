#include "Shader.hpp"

namespace vd::shader {
    std::string to_string(const ShaderType& shaderType) {
        switch (shaderType) {
            case eVertexShader: return "Vertex Shader";
            case eTessellationControlShader: return "Tessellation Control Shader";
            case eTessellationEvaluationShader: return "Tessellation Evaluation Shader";
            case eGeometryShader: return "Geometry Shader";
            case eComputeShader: return "Compute Shader";
            case eFragmentShader: return "Fragment Shader";
            default: return "Unknown";
        }
    }

    Shader::Shader()
        : program(0)
    {
    }

    void Shader::bind() const
    {
        glUseProgram(program);
    }

    void Shader::addUniform(const std::string& uniformName)
    {
        uint32_t location = glGetUniformLocation(program, uniformName.c_str());

        if (location == 0xFFFFFFFF)
        {
            vd::Logger::terminate("Could not find uniform " + uniformName, 1);
        }

        uniforms[uniformName] = location;
    }

    void Shader::addUniformBlock(const std::string& uniformBlockName)
    {
        uint32_t location = glGetUniformBlockIndex(program, uniformBlockName.c_str());

        if (location == 0xFFFFFFFF)
        {
            vd::Logger::terminate("Could not find uniform " + uniformBlockName, 1);
        }

        uniforms[uniformBlockName] = location;
    }

    void Shader::addShader(const std::string& shaderProgram, uint32_t type)
    {
        uint32_t shader = glCreateShader(type);

        if (shader == 0)
        {
            vd::Logger::terminate("Shader creation failed", 1);
        }

        const char* shaderSource = shaderProgram.c_str();
        glShaderSource(shader, 1, &shaderSource, NULL);

        glCompileShader(shader);

        int32_t success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            char infoLog[1024];
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);

            vd::Logger::terminate("[shader = " + to_string(static_cast<const ShaderType>(type)) + "] shader compilation error: " + infoLog, 1);
        }

        if (program == 0)
        {
            program = glCreateProgram();
            if (program == 0)
            {
                vd::Logger::terminate("Program creation failed", 1);
            }
        }

        glAttachShader(program, shader);
        glDeleteShader(shader);
    }

    void Shader::loadAndAddShader(const std::string& path, uint32_t type)
    {
        std::string shaderProgram;

        glslWrapper.parseGLSL(path, shaderProgram);

        addShader(shaderProgram, type);
    }

    void Shader::compileShader()
    {
        glLinkProgram(program);

        int32_t linkSuccess;
        glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);

        if (!linkSuccess)
        {
            char infoLog[1024];
            glGetProgramInfoLog(program, 1024, NULL, infoLog);

            vd::Logger::terminate("Shader linking error: " + std::string(infoLog), 1);
        }

        int32_t validateSuccess;
        glGetProgramiv(program, GL_VALIDATE_STATUS, &validateSuccess);

        if (!validateSuccess)
        {
            char infoLog[1024];
            glGetProgramInfoLog(program, 1024, NULL, infoLog);

            vd::Logger::terminate("Shader validation error: " + std::string(infoLog), 1);
        }
    }

    void Shader::setUniformi(const std::string& uniformName, int32_t value) const
    {
        auto it = uniforms.find(uniformName);
        if (it != uniforms.end())
            glUniform1i(it->second, value);
    }

    void Shader::setUniformf(const std::string& uniformName, float value) const
    {
        auto it = uniforms.find(uniformName);
        if (it != uniforms.end())
            glUniform1f(it->second, value);
    }

    void Shader::setUniform(const std::string& uniformName, const glm::vec2& value) const
    {
        auto it = uniforms.find(uniformName);
        if (it != uniforms.end())
            glUniform2f(it->second, value.x, value.y);
    }

    void Shader::setUniform(const std::string& uniformName, const glm::vec3& value) const
    {
        auto it = uniforms.find(uniformName);
        if (it != uniforms.end())
            glUniform3f(it->second, value.x, value.y, value.z);
    }

    void Shader::setUniform(const std::string& uniformName, const glm::vec4& value) const
    {
        auto it = uniforms.find(uniformName);
        if (it != uniforms.end())
            glUniform4f(it->second, value.x, value.y, value.z, value.w);
    }

    void Shader::setUniform(const std::string& uniformName, const glm::quat& value) const
    {
        auto it = uniforms.find(uniformName);
        if (it != uniforms.end())
            glUniform4f(it->second, value.x, value.y, value.z, value.w);
    }

    void Shader::setUniform(const std::string& uniformName, const glm::mat3& value) const
    {
        auto it = uniforms.find(uniformName);
        if (it != uniforms.end())
            glUniformMatrix3fv(it->second, 1, false, glm::value_ptr(value));
    }

    void Shader::setUniform(const std::string& uniformName, const glm::mat4& value) const
    {
        auto it = uniforms.find(uniformName);
        if (it != uniforms.end())
            glUniformMatrix4fv(it->second, 1, false, glm::value_ptr(value));
    }

    void Shader::bindUniformBlock(const std::string& uniformBlockName, uint32_t uniformBlockBinding) const
    {
        auto it = uniforms.find(uniformBlockName);
        if (it != uniforms.end())
            glUniformBlockBinding(program, it->second, uniformBlockBinding);
    }

    void Shader::bindFragDataLocation(const std::string& name, uint32_t index) const
    {
            glBindFragDataLocation(program, index, name.c_str());
    }

    uint32_t Shader::getProgram() const
    {
        return program;
    }

}
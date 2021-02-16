//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_SHADER_HPP
#define VD_GAME_ENGINE_SHADER_HPP

#include "GL.hpp"

#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <engine/exception/Exceptions.hpp>

namespace vd::gl {
    namespace exception {
        struct ShaderError : public CustomException {
            explicit ShaderError(const std::string& stage, const std::string& message);
        };

        struct UniformException : public CustomException {
            explicit UniformException(const std::string& message, const std::string& uniformName);
        };

        struct UniformLocationException : public UniformException {
            explicit UniformLocationException(const std::string& uniformName);
        };

        struct UniformNotLocatedException : public UniformException {
            explicit UniformNotLocatedException(const std::string& uniformName);
        };
    }

    class Shader {
    public:
        enum Type {
            eVertexShader = GL_VERTEX_SHADER,
            eGeometryShader = GL_GEOMETRY_SHADER,
            eFragmentShader = GL_FRAGMENT_SHADER,
            eTessellationControlShader = GL_TESS_CONTROL_SHADER,
            eTessellationEvaluationShader = GL_TESS_EVALUATION_SHADER,
            eComputeShader = GL_COMPUTE_SHADER
        };

        static std::string ToString(const Type& type);

        Shader();
        ~Shader();

        void Bind() const;
        static void Unbind();

        void AddShader(const std::string& source, const Type& type);
        void Compile() const;

        [[nodiscard]] uint32_t Program() const;

        void AddUniform(const std::string& uniformName);
        void AddUniformBlock(const std::string& uniformBlockName);

        void SetUniform(const std::string& uniformName, int value) const;
        void SetUniform(const std::string& uniformName, float value) const;
        void SetUniform(const std::string& uniformName, const glm::vec2& value) const;
        void SetUniform(const std::string& uniformName, const glm::vec3& value) const;
        void SetUniform(const std::string& uniformName, const glm::vec4& value) const;
        void SetUniform(const std::string& uniformName, const glm::quat& value) const;
        void SetUniform(const std::string& uniformName, const glm::mat3& value) const;
        void SetUniform(const std::string& uniformName, const glm::mat4& value) const;

        void PushUniform(const std::string& uniformName, int value);
        void PushUniform(const std::string& uniformName, float value);
        void PushUniform(const std::string& uniformName, const glm::vec2& value);
        void PushUniform(const std::string& uniformName, const glm::vec3& value);
        void PushUniform(const std::string& uniformName, const glm::vec4& value);
        void PushUniform(const std::string& uniformName, const glm::quat& value);
        void PushUniform(const std::string& uniformName, const glm::mat3& value);
        void PushUniform(const std::string& uniformName, const glm::mat4& value);

        void BindUniformBlock(const std::string& uniformBlockName, uint32_t uniformBlockBinding) const;
        void BindFragDataLocation(const std::string& name, uint32_t index) const;
    private:
        uint32_t m_Program;
        std::unordered_map<std::string, uint32_t> m_UniformMap;
    };
    typedef std::shared_ptr<Shader> ShaderPtr;
}


#endif //VD_GAME_ENGINE_SHADER_HPP

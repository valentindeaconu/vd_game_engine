//
// Created by vali on 4/16/21.
//

#ifndef VDGE_PROGRAMGLIMPL_HPP
#define VDGE_PROGRAMGLIMPL_HPP

#include <engine/gapi/Program.hpp>

#include <engine/gapi/gl/wrappers/Shader.hpp>

namespace vd::gl {
    class ProgramGLImpl : public gapi::Program {
    public:
        void Create(const std::vector<std::pair<gapi::GraphicPipelineStage, std::string>>&) override;

        void Use() override;
        void Discard() override;

        void SetEmpty(const std::string&) override;

        void Set(const std::string&, int) override;
        void Set(const std::string&, float) override;
        void Set(const std::string&, const glm::vec2&) override;
        void Set(const std::string&, const glm::vec3&) override;
        void Set(const std::string&, const glm::vec4&) override;
        void Set(const std::string&, const glm::quat&) override;
        void Set(const std::string&, const glm::mat3&) override;
        void Set(const std::string&, const glm::mat4&) override;

        void Release() override;
    private:
        static wrappers::Shader::Type ToShaderType(const gapi::GraphicPipelineStage& stage);

        std::unique_ptr<wrappers::Shader> m_Shader;
    };
}


#endif //VDGE_PROGRAMGLIMPL_HPP

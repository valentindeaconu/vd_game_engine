//
// Created by vali on 4/15/21.
//

#ifndef VDGE_PROGRAM_HPP
#define VDGE_PROGRAM_HPP

#include <vector>
#include <memory>
#include <string>
#include <any>
#include <glm/glm.hpp>

namespace vd::gapi {
    enum class GraphicPipelineStage {
        eVertex = 0,
        eTessellationControl,
        eTessellationEvaluation,
        eGeometry,
        eFragment,
        eCompute
    };

    class Program {
    public:
        virtual void Create(const std::vector<std::pair<GraphicPipelineStage, std::string>>&) = 0;

        virtual void Use() = 0;
        virtual void Discard() = 0;

        virtual void SetEmpty(const std::string&) = 0;

        virtual void Set(const std::string&, int) = 0;
        virtual void Set(const std::string&, float) = 0;
        virtual void Set(const std::string&, const glm::vec2&) = 0;
        virtual void Set(const std::string&, const glm::vec3&) = 0;
        virtual void Set(const std::string&, const glm::vec4&) = 0;
        virtual void Set(const std::string&, const glm::quat&) = 0;
        virtual void Set(const std::string&, const glm::mat3&) = 0;
        virtual void Set(const std::string&, const glm::mat4&) = 0;
        bool Set(const std::string&, const std::any&);

        virtual void Release() = 0;
    };
    typedef std::shared_ptr<Program>    ProgramPtr;
}


#endif //VDGE_PROGRAM_HPP

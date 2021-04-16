//
// Created by vali on 4/15/21.
//

#ifndef VDGE_IPROGRAM_HPP
#define VDGE_IPROGRAM_HPP

#include <memory>
#include <string>
#include <any>
#include <glm/glm.hpp>

namespace vd::gapi {
    class IProgram {
    public:
        virtual void Select() = 0;
        virtual void Unselect() = 0;

        virtual void Allocate(const std::string&) = 0;

        virtual void Set(const std::string&, int) = 0;
        virtual void Set(const std::string&, float) = 0;
        virtual void Set(const std::string&, const glm::vec2&) = 0;
        virtual void Set(const std::string&, const glm::vec3&) = 0;
        virtual void Set(const std::string&, const glm::vec4&) = 0;
        virtual void Set(const std::string&, const glm::quat&) = 0;
        virtual void Set(const std::string&, const glm::mat3&) = 0;
        virtual void Set(const std::string&, const glm::mat4&) = 0;
        bool Set(const std::string&, const std::any&);
    };
    typedef std::shared_ptr<IProgram>    IProgramPtr;
}


#endif //VDGE_IPROGRAM_HPP

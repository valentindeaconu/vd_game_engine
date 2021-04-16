//
// Created by vali on 4/16/21.
//

#include "IProgram.hpp"

namespace vd::gapi {

    bool IProgram::Set(const std::string& name, const std::any& value) {
        try { Set(name, std::any_cast<int>(value)); return true; } catch (const std::bad_any_cast& e) { }
        try { Set(name, std::any_cast<float>(value)); return true; } catch (const std::bad_any_cast& e) { }
        try { Set(name, std::any_cast<glm::vec2>(value)); return true; } catch (const std::bad_any_cast& e) { }
        try { Set(name, std::any_cast<glm::vec3>(value)); return true; } catch (const std::bad_any_cast& e) { }
        try { Set(name, std::any_cast<glm::vec4>(value)); return true; } catch (const std::bad_any_cast& e) { }
        try { Set(name, std::any_cast<glm::mat3>(value)); return true; } catch (const std::bad_any_cast& e) { }
        try { Set(name, std::any_cast<glm::mat4>(value)); return true; } catch (const std::bad_any_cast& e) { }

        return false;
    }



}
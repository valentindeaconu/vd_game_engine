//
// Created by Vali on 8/29/2020.
//

#ifndef VD_GAME_ENGINE_GUISHADER_HPP
#define VD_GAME_ENGINE_GUISHADER_HPP

#include <engine/shader/Shader.hpp>

#include <memory>

namespace mod::gui {
    class GuiShader : public vd::shader::Shader {
    public:
        GuiShader();
        ~GuiShader();

        void updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) override;
    };
    typedef std::shared_ptr<GuiShader>  GuiShaderPtr;
}

#endif //VD_GAME_ENGINE_GUISHADER_HPP

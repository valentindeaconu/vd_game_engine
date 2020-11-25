//
// Created by Vali on 8/29/2020.
//

#ifndef VD_GAME_ENGINE_GUISHADER_HPP
#define VD_GAME_ENGINE_GUISHADER_HPP

#include <engine/component/IEntityShader.hpp>

#include <engine/loader/ShaderLoader.hpp>

#include <memory>

namespace mod::gui {
    class GuiShader : public vd::component::IEntityShader {
    public:
        GuiShader();
        ~GuiShader();

        void InitUniforms(vd::object::EntityPtr pEntity) override;
        void UpdateUniforms(vd::object::EntityPtr pEntity, uint32_t meshIndex) override;
    private:
        void AddUniforms() override;
    };
    typedef std::shared_ptr<GuiShader>  GuiShaderPtr;
}

#endif //VD_GAME_ENGINE_GUISHADER_HPP

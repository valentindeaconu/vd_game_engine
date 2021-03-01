//
// Created by Vali on 8/29/2020.
//

#ifndef VD_GAME_ENGINE_GUISHADER_HPP
#define VD_GAME_ENGINE_GUISHADER_HPP

#include <engine/component/IEntityShader.hpp>

#include <engine/loader/ShaderLoader.hpp>

#include <memory>

namespace mod::gui {
    class GuiShader : public vd::component::IEntity2DShader {
    public:
        GuiShader();
        ~GuiShader();

        void InitUniforms(vd::object::Entity2DPtr pEntity) override;
        void UpdateUniforms(vd::object::Entity2DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) override;
    private:
        void AddUniforms() override;
    };
    typedef std::shared_ptr<GuiShader>  GuiShaderPtr;
}

#endif //VD_GAME_ENGINE_GUISHADER_HPP

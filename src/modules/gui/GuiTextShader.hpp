//
// Created by Vali on 2/18/2021.
//

#ifndef VDGE_GUITEXTSHADER_HPP
#define VDGE_GUITEXTSHADER_HPP

#include <engine/component/IEntityShader.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include <engine/injector/ObjectOfType.hpp>
#include <engine/injector/Injectable.hpp>

#include <engine/window/Window.hpp>

#include <memory>

#include "GuiText.hpp"

namespace mod::gui {
    class GuiTextShader : public vd::component::IEntity2DShader, public vd::injector::Injectable {
    public:
        GuiTextShader();

        void Link() override;

        void InitUniforms(vd::object::Entity2DPtr pEntity) override;
        void UpdateUniforms(vd::object::Entity2DPtr pEntity, uint32_t meshIndex) override;
    private:
        void AddUniforms() override;

        vd::window::WindowPtr m_pWindow;
    };
    typedef std::shared_ptr<GuiTextShader>  GuiTextShaderPtr;
}

#endif //VDGE_GUITEXTSHADER_HPP

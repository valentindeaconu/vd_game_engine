//
// Created by Vali on 8/29/2020.
//

#ifndef VD_GAME_ENGINE_GUIRENDERER_HPP
#define VD_GAME_ENGINE_GUIRENDERER_HPP

#include <engine/component/IEntityShader.hpp>
#include <engine/component/IRenderer.hpp>
#include <engine/api/gl/Context.hpp>

#include "GuiQuad.hpp"
#include "GuiText.hpp"

namespace mod::gui {
    class GuiRenderer : public vd::component::IRenderer {
    public:
        enum Type {
            eQuad = 0,
            eText
        };

        static const int kPriority = kDefaultPriority + 100;

        GuiRenderer(GuiQuadPtr guiQuadPtr, vd::component::IEntity2DShaderPtr shaderPtr);

        GuiRenderer(GuiTextPtr guiTextPtr, vd::component::IEntity2DShaderPtr shaderPtr);

        void OnInit() override;
        void OnUpdate() override;
        void OnRender(const params_t& params) override;
        void OnCleanUp() override;

    private:
        bool Precondition(const params_t& params) override;
        void Prepare() override;
        void Finish() override;

        Type                                m_Type;
        vd::component::IEntity2DShaderPtr   m_pShader;
        vd::object::Entity2DPtr             m_pGuiEntity;
    };
    typedef std::shared_ptr<GuiRenderer>	GuiRendererPtr;
}

#endif //VD_GAME_ENGINE_GUIRENDERER_HPP

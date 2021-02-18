//
// Created by Vali on 8/29/2020.
//

#ifndef VD_GAME_ENGINE_GUIRENDERER_HPP
#define VD_GAME_ENGINE_GUIRENDERER_HPP

#include <engine/component/IRenderer.hpp>

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

        GuiRenderer(GuiQuadPtr guiQuadPtr,
                    vd::component::IEntityShaderPtr shaderPtr,
                    vd::Consumer beforeExecution = vd::g_kEmptyConsumer,
                    vd::Consumer afterExecution = vd::g_kEmptyConsumer);

        GuiRenderer(GuiTextPtr guiTextPtr,
                    vd::component::IEntityShaderPtr shaderPtr,
                    vd::Consumer beforeExecution = vd::g_kEmptyConsumer,
                    vd::Consumer afterExecution = vd::g_kEmptyConsumer);

        void Init() override;
        void Update() override;
        void Render(const params_t& params) override;
        void CleanUp() override;

    private:
        bool IsReady() override;

        Type                    m_Type;
        vd::object::Entity2DPtr m_pGuiEntity;
    };
    typedef std::shared_ptr<GuiRenderer>	GuiRendererPtr;
}

#endif //VD_GAME_ENGINE_GUIRENDERER_HPP

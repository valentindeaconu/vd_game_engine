//
// Created by Vali on 8/29/2020.
//

#ifndef VD_GAME_ENGINE_GUIRENDERER_HPP
#define VD_GAME_ENGINE_GUIRENDERER_HPP

#include <engine/component/Renderer.hpp>

#include "GuiQuad.hpp"

namespace mod::gui {
    class GuiRenderer : public vd::component::Renderer {
    public:
        GuiRenderer();
        ~GuiRenderer();

        void Init() override;
        void Update() override;
        void Render(const vd::kernel::RenderingPass& renderingPass) override;
        void CleanUp() override;

        GuiQuadPtr& GetGuiQuad();
        [[nodiscard]] const GuiQuadPtr& GetGuiQuad() const;
        void SetGuiQuad(const GuiQuadPtr& guiQuadPtr);

    private:
        bool IsReady() override;

        bool m_Initialised;

        GuiQuadPtr m_GuiQuadPtr;
    };
    typedef std::shared_ptr<GuiRenderer>	GuiRendererPtr;
}

#endif //VD_GAME_ENGINE_GUIRENDERER_HPP

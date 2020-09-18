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

        void init() override;
        void update() override;
        void render(const vd::kernel::RenderingPass& renderingPass) override;
        void cleanUp() override;

        GuiQuadPtr& getGuiQuad();
        [[nodiscard]] const GuiQuadPtr& getGuiQuad() const;
        void setGuiQuad(const GuiQuadPtr& guiQuadPtr);

    private:
        bool isReady() override;

        bool initialised;

        GuiQuadPtr guiQuadPtr;
    };
    typedef std::shared_ptr<GuiRenderer>	GuiRendererPtr;
}

#endif //VD_GAME_ENGINE_GUIRENDERER_HPP

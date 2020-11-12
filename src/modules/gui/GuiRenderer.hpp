//
// Created by Vali on 8/29/2020.
//

#ifndef VD_GAME_ENGINE_GUIRENDERER_HPP
#define VD_GAME_ENGINE_GUIRENDERER_HPP

#include <engine/component/IRenderer.hpp>

#include "GuiQuad.hpp"

namespace mod::gui {
    class GuiRenderer : public vd::component::IRenderer {
    public:
        static const int kPriority = kDefaultPriority + 100;

        GuiRenderer(GuiQuadPtr guiQuadPtr,
                    vd::shader::ShaderPtr shaderPtr,
                    vd::Consumer beforeExecution = vd::g_kEmptyConsumer,
                    vd::Consumer afterExecution = vd::g_kEmptyConsumer);
        ~GuiRenderer();

        void Init() override;
        void Update() override;
        void Render(const params_t& params) override;
        void CleanUp() override;

    private:
        bool IsReady() override;

        GuiQuadPtr m_GuiQuadPtr;
    };
    typedef std::shared_ptr<GuiRenderer>	GuiRendererPtr;
}

#endif //VD_GAME_ENGINE_GUIRENDERER_HPP

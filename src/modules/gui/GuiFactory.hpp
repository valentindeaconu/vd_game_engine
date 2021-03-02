//
// Created by Vali on 11/19/2020.
//

#ifndef VD_GAME_ENGINE_GUIFACTORY_HPP
#define VD_GAME_ENGINE_GUIFACTORY_HPP

#include <engine/component/IModuleFactory.hpp>

#include <engine/loader/FontLoader.hpp>
#include <engine/context/Context.hpp>
#include <engine/time/TimeManager.hpp>

#include "GuiQuad.hpp"
#include "GuiText.hpp"
#include "GuiShader.hpp"
#include "GuiTextShader.hpp"
#include "GuiRenderer.hpp"

namespace mod::gui {
    class GuiFactory : public vd::component::IModuleFactory {
    public:
        GuiFactory();

        void Create(const vd::EnginePtr& pEngine) override;

    private:
        typedef std::function<vd::gl::Texture2DPtr()>   TextureGetter;

        void CreateGui(const vd::EnginePtr& pEngine,
                       const mod::gui::GuiQuad::TextureGetter& textureGetter,
                       const glm::vec2& position,
                       const glm::vec2& scale);

        void CreateText(const vd::EnginePtr& pEngine,
                        const std::string& text,
                        const glm::vec2& position,
                        float scale,
                        const glm::vec3& color);

        void CreateUpdatableText(const vd::EnginePtr& pEngine,
                                 const std::string& text,
                                 const UpdatableGuiText::UpdateConsumer& updateFn,
                                 const glm::vec2& position,
                                 float scale,
                                 const glm::vec3& color);

        vd::model::FontPtr          m_pFont;
        mod::gui::GuiShaderPtr      m_pGuiShader;
        mod::gui::GuiTextShaderPtr  m_pGuiTextShader;
    };
}


#endif //VD_GAME_ENGINE_GUIFACTORY_HPP

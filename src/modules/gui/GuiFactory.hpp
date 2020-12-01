//
// Created by Vali on 11/19/2020.
//

#ifndef VD_GAME_ENGINE_GUIFACTORY_HPP
#define VD_GAME_ENGINE_GUIFACTORY_HPP

#include <engine/component/IModuleFactory.hpp>

#include "GuiQuad.hpp"
#include "GuiShader.hpp"
#include "GuiRenderer.hpp"

namespace mod::gui {
    class GuiFactory : public vd::component::IModuleFactory {
    public:
        void Create(const vd::EnginePtr& pEngine) override;

    private:
        typedef std::function<vd::gl::Texture2DPtr()>   TextureGetter;

        static void CreateGui(const vd::EnginePtr& pEngine,
                              const mod::gui::GuiQuad::TextureGetter& textureGetter,
                              const glm::vec2& position,
                              const glm::vec2& scale);
    };
}


#endif //VD_GAME_ENGINE_GUIFACTORY_HPP

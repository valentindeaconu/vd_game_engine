//
// Created by Vali on 11/19/2020.
//

#include "GuiFactory.hpp"

#include <modules/terrain/Terrain.hpp>
#include <modules/water/Water.hpp>
#include <modules/shadow/ShadowManager.hpp>

namespace mod::gui {

    void GuiFactory::Create(const vd::EnginePtr& pEngine) {
        /// Reflection Texture
        /* CreateGui(pEngine,
                  [w = vd::ObjectOfType<mod::water::Water>::Find()]() {
                    return w->ReflectionFramebuffer()->GetColorTexture();
                  },
                  glm::vec2(0.75f, 0.75f),
                  glm::vec2(0.250f, 0.250f)); */

        /// Shadow Texture
        /* CreateGui(pEngine,
                  [s = vd::ObjectOfType<mod::shadow::ShadowManager>::Find()]() {
                      return s->ShadowTexture();
                  },
                  glm::vec2(-0.75f, -0.75f),
                  glm::vec2(0.250f, 0.250f)); */

        /* /// Terrain Height Map
        CreateGui(pEngine,
                  [t = vd::ObjectOfType<mod::terrain::Terrain>::Find()]() {
                      return t->HeightMap();
                  },
                  glm::vec2(0.75f, 0.75f),
                  glm::vec2(0.250f, 0.250f)); */

        /// Terrain Normal Map
        CreateGui(pEngine,
                  [t = vd::ObjectOfType<mod::terrain::Terrain>::Find()]() {
                      return t->NormalMap();
                  },
                  glm::vec2(0.75f, -0.75f),
                  glm::vec2(0.250f, 0.250f));
    }

    void GuiFactory::CreateGui(const vd::EnginePtr& pEngine,
                               const mod::gui::GuiQuad::TextureGetter& textureGetter,
                               const glm::vec2& position,
                               const glm::vec2& scale) {

        GuiQuadPtr pGuiQuad = std::make_shared<GuiQuad>(textureGetter, position, scale);
        GuiShaderPtr pGuiShader = std::make_shared<GuiShader>();
        GuiRendererPtr pGuiRenderer = std::make_shared<GuiRenderer>(pGuiQuad,
                                                                    pGuiShader,
                                                                    []() { glFrontFace(GL_CCW); },
                                                                    []() { glFrontFace(GL_CW); });

        pEngine->Subscribe(pGuiRenderer, GuiRenderer::kPriority);
    }

};

//
// Created by Vali on 11/19/2020.
//

#include "GuiFactory.hpp"

#include <modules/terrain/Terrain.hpp>
#include <modules/water/Water.hpp>
#include <modules/shadow/ShadowManager.hpp>

namespace mod::gui {

    GuiFactory::GuiFactory() {
        m_pFont = vd::loader::FontLoader::Load("./resources/fonts/Roboto/Roboto-Regular.ttf", 32);
        m_pGuiShader = std::make_shared<GuiShader>();
        m_pGuiTextShader = std::make_shared<GuiTextShader>();
    }

    void GuiFactory::Create(const vd::EnginePtr& pEngine) {
        /// Create & register rendering pass
        vd::component::RenderingPass rpGUI("GUI",
                                           1000,
                                           nullptr,
                                           vd::g_kEmptyPredicate,
                                           []() { glDisable(GL_DEPTH_TEST); },
                                           []() { glEnable(GL_DEPTH_TEST); });
        rpGUI.Clearing() = false;
        pEngine->Add(rpGUI);

        /// Refraction Texture
        /* CreateGui(pEngine,
                  [w = vd::ObjectOfType<mod::water::Water>::Find()]() {
                    return w->RefractionFramebuffer()->GetColorTexture();
                  },
                  glm::vec2(0.75f, 0.75f),
                  glm::vec2(0.250f, 0.250f));

        CreateGui(pEngine,
                  [w = vd::ObjectOfType<mod::water::Water>::Find()]() {
                      return w->RefractionFramebuffer()->DepthTexture();
                  },
                  glm::vec2(-0.75f, -0.75f),
                  glm::vec2(0.250f, 0.250f)); */

        /// Shadow Texture
        /* CreateGui(pEngine,
                  [s = vd::ObjectOfType<mod::shadow::ShadowManager>::Find()]() {
                      return s->ShadowTexture();
                  },
                  glm::vec2(0.75f, 0.75f),
                  glm::vec2(0.250f, 0.250f)); */

        /// Terrain Height Map
        /* CreateGui(pEngine,
                  [t = vd::ObjectOfType<mod::terrain::Terrain>::Find()]() {
                      return t->HeightMap();
                  },
                  glm::vec2(0.75f, 0.75f),
                  glm::vec2(0.250f, 0.250f)); */

        /// Terrain Normal Map
        /* CreateGui(pEngine,
                  [t = vd::ObjectOfType<mod::terrain::Terrain>::Find()]() {
                      return t->NormalMap();
                  },
                  glm::vec2(0.75f, -0.75f),
                  glm::vec2(0.250f, 0.250f)); */

        /// Scene FBO
        /* CreateGui(pEngine,
                  [ctx = vd::ObjectOfType<vd::context::Context>::Find()]() {
                      return ctx->SceneFrameBuffer()->ColorTexture();
                  },
                  glm::vec2(0.75f, -0.75f),
                  glm::vec2(0.250f, 0.250f));*/

        CreateUpdatableText(
            pEngine,
            "0",
            [
                ctx = vd::ObjectOfType<vd::context::Context>::Find(),
                w = vd::ObjectOfType<vd::window::Window>::Find()
            ](UpdatableGuiText& el) {
                bool shouldRebuild = false;
                if (!el.Contains("U")) {
                    el["U"] = "true";
                    el.Position() = glm::vec2(5.0f, w->Height() - 30.0f);
                    shouldRebuild = true;
                }

                std::string now = std::to_string(ctx->FPS());
                if (now != el.Text()) {
                    el.Text() = now;
                    shouldRebuild = true;
                }

                if (shouldRebuild) {
                    el.Rebuild();
                }
            },
            glm::vec2(200, 200),
            1.0f,
            glm::vec3(0.0f, 0.0f, 0.0f) // glm::vec3(0.0039f, 0.4745f, 0.4353f)
        );

        CreateUpdatableText(
                pEngine,
                "00:00",
                [
                        t = vd::ObjectOfType<vd::time::TimeManager>::Find(),
                        w = vd::ObjectOfType<vd::window::Window>::Find()
                ](UpdatableGuiText& el) {
                    bool shouldRebuild = false;
                    if (!el.Contains("U")) {
                        el["U"] = "true";
                        el.Position() = glm::vec2(w->Width() - 80.0f, w->Height() - 30.0f);
                        shouldRebuild = true;
                    }

                    std::string now = t->CurrentTime()->ToString();
                    if (now != el.Text()) {
                        el.Text() = now;
                        shouldRebuild = true;
                    }

                    if (shouldRebuild) {
                        el.Rebuild();
                    }
                },
                glm::vec2(200, 200),
                1.0f,
                glm::vec3(0.0f, 0.0f, 0.0f) // glm::vec3(0.0039f, 0.4745f, 0.4353f)
        );
    }

    void GuiFactory::CreateGui(const vd::EnginePtr& pEngine,
                               const mod::gui::GuiQuad::TextureGetter& textureGetter,
                               const glm::vec2& position,
                               const glm::vec2& scale) {

        GuiQuadPtr pGuiQuad = std::make_shared<GuiQuad>(textureGetter, position, scale);
        GuiRendererPtr pGuiRenderer = std::make_shared<GuiRenderer>(pGuiQuad,
                                                                    m_pGuiShader,
                                                                    []() { glFrontFace(GL_CCW); },
                                                                    []() { glFrontFace(GL_CW); });

        pEngine->Subscribe(pGuiRenderer, GuiRenderer::kPriority);
    }

    void GuiFactory::CreateText(const vd::EnginePtr& pEngine,
                                const std::string& text,
                                const glm::vec2& position,
                                float scale,
                                const glm::vec3& color) {
        GuiTextPtr pGuiText = std::make_shared<GuiText>(text, m_pFont, position, scale, color);

        vd::Consumer before = []() {
            glFrontFace(GL_CW);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_DEPTH_TEST);
        };

        vd::Consumer after = []() {
            glFrontFace(GL_CCW);
            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
        };

        GuiRendererPtr pGuiRenderer = std::make_shared<GuiRenderer>(pGuiText,
                                                                    m_pGuiTextShader,
                                                                    before,
                                                                    after);

        pEngine->Subscribe(pGuiRenderer, GuiRenderer::kPriority);
    }

    void GuiFactory::CreateUpdatableText(const vd::EnginePtr& pEngine,
                                         const std::string& text,
                                         const UpdatableGuiText::UpdateConsumer& updateFn,
                                         const glm::vec2& position,
                                         float scale,
                                         const glm::vec3& color)
    {
        UpdatableGuiTextPtr pGuiText = std::make_shared<UpdatableGuiText>(text, m_pFont, updateFn, position, scale, color);

        vd::Consumer before = []() {
            glFrontFace(GL_CW);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_DEPTH_TEST);
        };

        vd::Consumer after = []() {
            glFrontFace(GL_CCW);
            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
        };

        GuiRendererPtr pGuiRenderer = std::make_shared<GuiRenderer>(pGuiText,
                                                                    m_pGuiTextShader,
                                                                    before,
                                                                    after);

        pEngine->Subscribe(pGuiRenderer, GuiRenderer::kPriority);
    }

};

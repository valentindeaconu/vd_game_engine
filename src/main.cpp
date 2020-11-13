//
//	VD GAME ENGINE
//	main.cpp
//

#include <engine/kernel/Engine.hpp>
#include <engine/kernel/EngineFactory.hpp>

// player
#include <modules/player/Player.hpp>
#include <modules/player/PlayerShader.hpp>
#include <modules/player/PlayerRenderer.hpp>

// sky
#include <modules/sky/Sky.hpp>
#include <modules/sky/SkyShader.hpp>
#include <modules/sky/SkyRenderer.hpp>

// terrain
#include <modules/terrain/TerrainRenderer.hpp>
#include <modules/terrain/Terrain.hpp>
#include <modules/terrain/TerrainShader.hpp>

// shadow
#include <modules/shadow/ShadowManager.hpp>
#include <modules/shadow/ShadowShader.hpp>

// object generator
#include <modules/sobj/StaticObjectRenderer.hpp>
#include <modules/sobj/StaticObjectPlacer.hpp>
#include <modules/sobj/StaticObjectShader.hpp>

// gui
#include <modules/gui/GuiQuad.hpp>
#include <modules/gui/GuiShader.hpp>
#include <modules/gui/GuiRenderer.hpp>

// water
#include <modules/water/Water.hpp>
#include <modules/water/WaterRenderer.hpp>
#include <modules/water/WaterShader.hpp>

// object finder
#include <engine/kernel/ObjectOfType.hpp>

#include <engine/misc/Properties.hpp>

const vd::Consumer g_CCWConsumer = []() { glFrontFace(GL_CCW); };
const vd::Consumer g_CWConsumer = []() { glFrontFace(GL_CW); };

mod::terrain::TerrainPtr createTerrain(vd::EnginePtr& enginePtr);
mod::player::PlayerPtr createPlayer(vd::EnginePtr& enginePtr);
mod::sky::SkyPtr createSky(vd::EnginePtr& enginePtr);

void createShadow(vd::EnginePtr& enginePtr);

void createAndPlaceStaticObjects(vd::EnginePtr& enginePtr);

mod::water::WaterPtr createWater(vd::EnginePtr& enginePtr);

[[maybe_unused]] void createGUI(vd::EnginePtr& enginePtr,
                                const vd::gl::Texture2DPtr& texturePtr,
                                const glm::vec2& position,
                                const glm::vec2& scale);

int main(int argc, char ** argv) {
    // Read global properties
    auto globalPropertiesPtr = vd::misc::Properties::Create<vd::misc::Properties::eFile>("./resources/global.properties");
    vd::ObjectOfType<vd::misc::Properties>::Provide(globalPropertiesPtr);

    /// Engine creation
    vd::EnginePtr enginePtr = vd::EngineFactory::Create();

	/// Mods
    createTerrain(enginePtr);

    createSky(enginePtr);

	createPlayer(enginePtr);

    createAndPlaceStaticObjects(enginePtr);

    createShadow(enginePtr);

	/// Water must be the last element to draw, but before GUIs
	createWater(enginePtr);

    /// Debugging GUIs
    /*createGUI(enginePtr,
              enginePtr->getShadowManager()->getShadowTexture(),
              glm::vec2(-0.75f, -0.75f),
              glm::vec2(0.250f, 0.250f));

    createGUI(enginePtr,
              waterPtr->GetReflectionFramebuffer()->GetColorTexture(),
              glm::vec2(0.75f, 0.75f),
              glm::vec2(0.250f, 0.250f));

    createGUI(enginePtr,
              terrainPtr->GetTerrainConfig()->getNormalMap(),
              glm::vec2(0.75f, 0.75f),
              glm::vec2(0.250f, 0.250f));*/

	/// Engine Init
	enginePtr->Init();

	/// Starting Main Loop
	enginePtr->Start();

	return 0;
}

mod::terrain::TerrainPtr createTerrain(vd::EnginePtr& enginePtr) {
    mod::terrain::TerrainPtr terrainPtr =
            std::make_shared<mod::terrain::Terrain>("./resources/terrain.properties");

    mod::terrain::TerrainShaderPtr terrainShaderPtr = std::make_shared<mod::terrain::TerrainShader>();

    mod::terrain::TerrainRendererPtr terrainRendererPtr =
            std::make_shared<mod::terrain::TerrainRenderer>(terrainPtr, terrainShaderPtr, g_CCWConsumer, g_CWConsumer);

    enginePtr->Subscribe(terrainRendererPtr, mod::terrain::TerrainRenderer::kDefaultPriority);

    vd::ObjectOfType<mod::terrain::Terrain>::Provide(terrainPtr);

    return terrainPtr;
}

mod::player::PlayerPtr createPlayer(vd::EnginePtr& enginePtr) {
    mod::player::PlayerPtr playerPtr = std::make_shared<mod::player::Player>();
    mod::player::PlayerShaderPtr playerShaderPtr = std::make_shared<mod::player::PlayerShader>();

    mod::player::PlayerRendererPtr playerRendererPtr =
            std::make_shared<mod::player::PlayerRenderer>(playerPtr, playerShaderPtr, g_CCWConsumer, g_CWConsumer);

    enginePtr->Subscribe(playerRendererPtr, mod::player::PlayerRenderer::kDefaultPriority);

    vd::ObjectOfType<mod::player::Player>::Provide(playerPtr);

    return playerPtr;
}

mod::sky::SkyPtr createSky(vd::EnginePtr& enginePtr) {
    vd::Consumer before = []() {
        glDepthFunc(GL_LEQUAL);
        glFrontFace(GL_CCW);
    };

    vd::Consumer after = []() {
        glDepthFunc(GL_LESS);
        glFrontFace(GL_CW);
    };

    mod::sky::SkyPtr skyPtr = std::make_shared<mod::sky::Sky>();
    mod::sky::SkyShaderPtr skyShaderPtr = std::make_shared<mod::sky::SkyShader>();

    mod::sky::SkyRendererPtr skyRendererPtr =
            std::make_shared<mod::sky::SkyRenderer>(skyPtr, skyShaderPtr, before, after);

    enginePtr->Subscribe(skyRendererPtr, mod::sky::SkyRenderer::kDefaultPriority);

    return skyPtr;
}

void createShadow(vd::EnginePtr& enginePtr) {
    mod::shadow::ShadowManagerPtr shadowManagerPtr = std::make_shared<mod::shadow::ShadowManager>();

    enginePtr->Subscribe(shadowManagerPtr, mod::shadow::ShadowManager::kDefaultPriority);

    vd::ObjectOfType<mod::shadow::ShadowManager>::Provide(shadowManagerPtr);

    vd::component::RenderingPass shadowRenderingPass(
        "Shadow",
        10,
        shadowManagerPtr->FrameBuffer(),
        vd::g_kEmptyPredicate,
        []() { glDisable(GL_CULL_FACE); },
        []() { glEnable(GL_CULL_FACE); }
    );

    enginePtr->Add(shadowRenderingPass);

    mod::shadow::ShadowShaderPtr shadowShaderPtr = std::make_shared<mod::shadow::ShadowShader>();
    vd::ObjectOfType<mod::shadow::ShadowShader>::Provide(shadowShaderPtr);
}

void createAndPlaceStaticObjects(vd::EnginePtr& enginePtr) {
    mod::sobj::StaticObjectPlacerPtr staticObjectPlacerPtr =
            std::make_shared<mod::sobj::StaticObjectPlacer>(7000, 10.0f);
    mod::sobj::StaticObjectShaderPtr staticObjectShaderPtr = std::make_shared<mod::sobj::StaticObjectShader>();

    mod::sobj::StaticObjectRendererPtr staticObjectRendererPtr =
            std::make_shared<mod::sobj::StaticObjectRenderer>(staticObjectPlacerPtr, staticObjectShaderPtr, g_CCWConsumer, g_CWConsumer);

    enginePtr->Subscribe(staticObjectRendererPtr);
}

mod::water::WaterPtr createWater(vd::EnginePtr& enginePtr) {
    mod::water::WaterPtr waterPtr = std::make_shared<mod::water::Water>("./resources/water.properties");
    mod::water::WaterShaderPtr waterShaderPtr = std::make_shared<mod::water::WaterShader>();

    vd::Consumer before = []() {
        glFrontFace(GL_CCW);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    };

    vd::Consumer after = []() {
        glFrontFace(GL_CW);
        glDisable(GL_BLEND);
    };

    mod::water::WaterRendererPtr waterRendererPtr =
            std::make_shared<mod::water::WaterRenderer>(waterPtr, waterShaderPtr, before, after);

    vd::Predicate passPrecondition = [w = waterPtr.get()]() {
        return vd::ObjectOfType<vd::camera::Camera>::Find()->Position().y > w->GetHeight();
    };

    vd::Consumer reflectionPassBefore = [e = enginePtr.get(), w = waterPtr.get()] {
        e->setClipPlane(glm::vec4(0.0f, 1.0f, 0.0f, -w->GetHeight() + 1.0f));
        glEnable(GL_CLIP_DISTANCE0);

        vd::ObjectOfType<vd::camera::Camera>::Find()->Reflect(vd::camera::Camera::eY, w->GetHeight());
    };

    vd::Consumer reflectionPassAfter = [e = enginePtr.get(), w = waterPtr.get()] {
        e->setClipPlane(glm::vec4(0.0f));
        glDisable(GL_CLIP_DISTANCE0);

        vd::ObjectOfType<vd::camera::Camera>::Find()->Reflect(vd::camera::Camera::eY, w->GetHeight());
    };

    vd::component::RenderingPass reflectionPass(
        "Reflection",
        20,
        waterPtr->GetReflectionFramebuffer(),
        passPrecondition,
        reflectionPassBefore,
        reflectionPassAfter
    );
    enginePtr->Add(reflectionPass);

    vd::Consumer refractionPassBefore = [e = enginePtr.get(), w = waterPtr.get()] {
        e->setClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, w->GetHeight()));
        glEnable(GL_CLIP_DISTANCE0);
    };

    vd::Consumer refractionPassAfter = [e = enginePtr.get()] {
        e->setClipPlane(glm::vec4(0.0f));
        glDisable(GL_CLIP_DISTANCE0);
    };

    vd::component::RenderingPass refractionPass(
        "Refraction",
        20,
        waterPtr->GetRefractionFramebuffer(),
        passPrecondition,
        refractionPassBefore,
        refractionPassAfter
    );

    enginePtr->Add(refractionPass);

    /// Water must be the last element to draw, but before GUIs
    enginePtr->Subscribe(waterRendererPtr, mod::water::WaterRenderer::kPriority);

    return waterPtr;
}

[[maybe_unused]] void createGUI(vd::EnginePtr& enginePtr,
               const vd::gl::Texture2DPtr& texturePtr,
               const glm::vec2& position,
               const glm::vec2& scale) {
    mod::gui::GuiQuadPtr guiQuadPtr =
            std::make_shared<mod::gui::GuiQuad>(texturePtr, position, scale);
    mod::gui::GuiShaderPtr guiShaderPtr = std::make_shared<mod::gui::GuiShader>();

    mod::gui::GuiRendererPtr guiRendererPtr =
            std::make_shared<mod::gui::GuiRenderer>(guiQuadPtr, guiShaderPtr, g_CCWConsumer, g_CWConsumer);

    /// GUIs must be the last elements to draw
    enginePtr->Subscribe(guiRendererPtr, mod::gui::GuiRenderer::kPriority);
}
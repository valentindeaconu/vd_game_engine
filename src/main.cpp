//
//	VD GAME ENGINE
//	main.cpp
//

#include <engine/kernel/EngineBlock.hpp>

// configs
#include <engine/config/MetaConfig.hpp>

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

mod::terrain::TerrainPtr createTerrain(vd::EnginePtr& enginePtr);
mod::player::PlayerPtr createPlayer(vd::EnginePtr& enginePtr, mod::terrain::TerrainPtr& terrainPtr);
mod::sky::SkyPtr createSky(vd::EnginePtr& enginePtr);

void createAndPlaceStaticObjects(vd::EnginePtr& enginePtr);

mod::water::WaterPtr createWater(vd::EnginePtr& enginePtr);

void createGUI(vd::EnginePtr& enginePtr,
               vd::model::Texture2DPtr& texturePtr,
               const glm::vec2& position,
               const glm::vec2& scale);

int main(int argc, char ** argv) {

    /// Engine creation
	vd::EnginePtr enginePtr = std::make_shared<vd::Engine>();
	enginePtr->setup(1280, 720, "VD Game Engine");

	/// Mods
	mod::terrain::TerrainPtr terrainPtr = createTerrain(enginePtr);

    mod::sky::SkyPtr skyPtr = createSky(enginePtr);

	mod::player::PlayerPtr playerPtr = createPlayer(enginePtr, terrainPtr);

    createAndPlaceStaticObjects(enginePtr);

	/// Water must be the last element to draw, but before GUIs
	mod::water::WaterPtr waterPtr = createWater(enginePtr);

	/// Engine init
    enginePtr->init();

    /// Debugging GUIs
    /*createGUI(enginePtr,
              enginePtr->getShadowManager()->getShadowTexture(),
              glm::vec2(-0.75f, -0.75f),
              glm::vec2(0.250f, 0.250f));*/

    createGUI(enginePtr,
              waterPtr->getRefractionFramebuffer()->getColorTexture(),
              glm::vec2(-0.75f, -0.75f),
              glm::vec2(0.250f, 0.250f));

    createGUI(enginePtr,
              waterPtr->getReflectionFramebuffer()->getColorTexture(),
              glm::vec2(0.75f, 0.75f),
              glm::vec2(0.250f, 0.250f));

	/// Starting Main Loop
	enginePtr->start();

	return 0;
}

mod::terrain::TerrainPtr createTerrain(vd::EnginePtr& enginePtr) {
    const vd::config::MetaConfigPtr ccwConfigPtr =
            std::make_shared<vd::config::MetaConfig>([]() { glFrontFace(GL_CCW); },
                                                     []() { glFrontFace(GL_CW); });

    mod::terrain::TerrainPtr terrainPtr =
            std::make_shared<mod::terrain::Terrain>(enginePtr, "./resources/terrain_settings.cfg");
    mod::terrain::TerrainShaderPtr terrainShaderPtr = std::make_shared<mod::terrain::TerrainShader>();

    mod::terrain::TerrainRendererPtr terrainRendererPtr = std::make_shared<mod::terrain::TerrainRenderer>();
    terrainRendererPtr->setTerrain(terrainPtr);
    terrainRendererPtr->setRenderConfig(ccwConfigPtr);
    terrainRendererPtr->setShader(terrainShaderPtr);

    enginePtr->getWorker()->subscribe(terrainRendererPtr);

    return terrainPtr;
}

mod::player::PlayerPtr createPlayer(vd::EnginePtr& enginePtr, mod::terrain::TerrainPtr& terrainPtr) {
    const vd::config::MetaConfigPtr ccwConfigPtr =
            std::make_shared<vd::config::MetaConfig>([]() { glFrontFace(GL_CCW); },
                                                     []() { glFrontFace(GL_CW); });

    mod::player::PlayerPtr playerPtr = std::make_shared<mod::player::Player>(enginePtr, terrainPtr);
    mod::player::PlayerShaderPtr playerShaderPtr = std::make_shared<mod::player::PlayerShader>();

    mod::player::PlayerRendererPtr playerRendererPtr = std::make_shared<mod::player::PlayerRenderer>();
    playerRendererPtr->setPlayer(playerPtr);
    playerRendererPtr->setRenderConfig(ccwConfigPtr);
    playerRendererPtr->setShader(playerShaderPtr);

    enginePtr->getWorker()->subscribe(playerRendererPtr);


    // set camera follows this entity
    // cameraInitParameters.entityPtr = playerPtr;

    // with an offset of 1 on Y axis
    // "don't look at shoes, look at chest"
    // cameraInitParameters.playerPositionOffset = glm::vec3(0.0f, 1.0f, 0.0f);

    // using this terrain
    // cameraInitParameters.terrainPtr = terrainPtr;

    return playerPtr;
}

mod::sky::SkyPtr createSky(vd::EnginePtr& enginePtr) {
    vd::config::MetaConfigPtr skyConfigPtr = std::make_shared<vd::config::MetaConfig>([]() {
        glDepthFunc(GL_LEQUAL);
        glFrontFace(GL_CCW);
    }, []() {
        glDepthFunc(GL_LESS);
        glFrontFace(GL_CW);
    });

    mod::sky::SkyPtr skyPtr = std::make_shared<mod::sky::Sky>(enginePtr);
    mod::sky::SkyShaderPtr skyShaderPtr = std::make_shared<mod::sky::SkyShader>();

    mod::sky::SkyRendererPtr skyRendererPtr = std::make_shared<mod::sky::SkyRenderer>();
    skyRendererPtr->setSky(skyPtr);
    skyRendererPtr->setRenderConfig(skyConfigPtr);
    skyRendererPtr->setShader(skyShaderPtr);

    enginePtr->getWorker()->subscribe(skyRendererPtr);

    return skyPtr;
}

void createAndPlaceStaticObjects(vd::EnginePtr& enginePtr) {
    mod::sobj::StaticObjectPlacerPtr staticObjectPlacerPtr =
            std::make_shared<mod::sobj::StaticObjectPlacer>(terrainPtr, 7000, 10.0f);
    mod::sobj::StaticObjectShaderPtr staticObjectShaderPtr = std::make_shared<mod::sobj::StaticObjectShader>();

    mod::sobj::StaticObjectRendererPtr staticObjectRendererPtr = std::make_shared<mod::sobj::StaticObjectRenderer>();
    staticObjectRendererPtr->setStaticObjectPlacer(staticObjectPlacerPtr);
    staticObjectRendererPtr->setRenderConfig(ccwConfigPtr);
    staticObjectRendererPtr->setShader(staticObjectShaderPtr);

    enginePtr->getWorker()->subscribe(staticObjectRendererPtr);
}

mod::water::WaterPtr createWater(vd::EnginePtr& enginePtr) {
    mod::water::WaterPtr waterPtr = std::make_shared<mod::water::Water>(enginePtr, "./resources/water_settings.cfg");
    mod::water::WaterShaderPtr waterShaderPtr = std::make_shared<mod::water::WaterShader>();

    vd::config::MetaConfigPtr waterConfigPtr = std::make_shared<vd::config::MetaConfig>([]() {
        glFrontFace(GL_CCW);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }, []() {
        glFrontFace(GL_CW);
        glDisable(GL_BLEND);
    });

    mod::water::WaterRendererPtr waterRendererPtr = std::make_shared<mod::water::WaterRenderer>();
    waterRendererPtr->setWater(waterPtr);
    waterRendererPtr->setRenderConfig(waterConfigPtr);
    waterRendererPtr->setShader(waterShaderPtr);

    auto preconditionFunc = [&]() {
        return enginePtr->getCamera()->getPosition().y > waterPtr->getHeight();
    };

    auto reflectionEnableFunc = [&]{
        enginePtr->setClipPlane(glm::vec4(0.0f, 1.0f, 0.0f, -waterPtr->getHeight() + 1.0f));
        glEnable(GL_CLIP_DISTANCE0);

        enginePtr->getCamera()->reflect(waterPtr->getHeight());
    };

    auto reflectionDisableFunc = [&]() {
        enginePtr->setClipPlane(glm::vec4(0.0f));
        glDisable(GL_CLIP_DISTANCE0);

        enginePtr->getCamera()->reflect(waterPtr->getHeight());
    };

    auto reflectionCfgPtr = std::make_shared<vd::config::MetaConfig>(reflectionEnableFunc, reflectionDisableFunc);

    enginePtr->addRenderingFramebuffer(waterPtr->getReflectionFramebuffer(),
                                       preconditionFunc,
                                       reflectionCfgPtr,
                                       vd::kernel::RenderingPass::eReflection);

    auto refractionEnableFunc = [&]{
        enginePtr->setClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, waterPtr->getHeight()));
        glEnable(GL_CLIP_DISTANCE0);
    };

    auto refractionDisableFunc = [&]() {
        enginePtr->setClipPlane(glm::vec4(0.0f));
        glDisable(GL_CLIP_DISTANCE0);
    };

    auto refractionCfgPtr = std::make_shared<vd::config::MetaConfig>(refractionEnableFunc, refractionDisableFunc);

    enginePtr->addRenderingFramebuffer(waterPtr->getRefractionFramebuffer(),
                                       preconditionFunc,
                                       refractionCfgPtr,
                                       vd::kernel::RenderingPass::eRefraction);

    enginePtr->getWorker()->subscribe(waterRendererPtr);
}

void createGUI(vd::EnginePtr& enginePtr,
               vd::model::Texture2DPtr& texturePtr,
               const glm::vec2& position,
               const glm::vec2& scale) {
    mod::gui::GuiQuadPtr guiQuadPtr =
            std::make_shared<mod::gui::GuiQuad>(enginePtr, texturePtr, position, scale);
    mod::gui::GuiShaderPtr guiShaderPtr = std::make_shared<mod::gui::GuiShader>();

    mod::gui::GuiRendererPtr  guiRendererPtr = std::make_shared<mod::gui::GuiRenderer>();
    guiRendererPtr->setRenderConfig(ccwConfigPtr);
    guiRendererPtr->setGuiQuad(guiQuadPtr);
    guiRendererPtr->setShader(guiShaderPtr);

    enginePtr->getWorker()->subscribe(guiRendererPtr);
}
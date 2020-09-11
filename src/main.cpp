//
//	VD GAME ENGINE
//	main.cpp
//

#include <engine/kernel/EngineBlock.hpp>

// dependency injector includes
#include <engine/component/Renderer.hpp>
#include <engine/component/impl/EntityRenderer.hpp>

// player
#include <modules/player/Player.hpp>
#include <modules/player/PlayerShader.hpp>

// ground
#include <modules/sky/Sky.hpp>
#include <modules/sky/SkyShader.hpp>

// terrain
#include <modules/terrain/TerrainRenderer.hpp>
#include <modules/terrain/Terrain.hpp>
#include <modules/terrain/TerrainShader.hpp>

// entity camera parameters
#include <engine/core/impl/EntityCamera.hpp>

// object generator
#include <modules/sobj/StaticObjectRenderer.hpp>
#include <modules/sobj/StaticObjectPlacer.hpp>
#include <modules/sobj/StaticObjectShader.hpp>

// gui
#include <modules/gui/GuiQuad.hpp>
#include <modules/gui/GuiShader.hpp>
#include <modules/gui/GuiRenderer.hpp>

int main(int argc, char ** argv)
{
	vd::EnginePtr enginePtr = std::make_shared<vd::Engine>();
	enginePtr->setup(1280, 720, "VD Game Engine");

	// Dependency Injector part

	// camera parameters
	vd::core::EntityCameraInitParameters cameraInitParameters;

	// renderer config init
	vd::component::CWConfigPtr cwConfigPtr = std::make_shared<vd::component::CWConfig>();
	vd::component::CCWConfigPtr ccwConfigPtr = std::make_shared<vd::component::CCWConfig>();

	mod::terrain::TerrainPtr terrainPtr = 
		std::make_shared<mod::terrain::Terrain>(enginePtr, "./resources/terrain_settings.cfg");
	mod::terrain::TerrainShaderPtr terrainShaderPtr = std::make_shared<mod::terrain::TerrainShader>();
		
	mod::terrain::TerrainRendererPtr terrainRendererPtr = std::make_shared<mod::terrain::TerrainRenderer>();
	terrainRendererPtr->setTerrain(terrainPtr);
	terrainRendererPtr->setRenderConfig(ccwConfigPtr);
	terrainRendererPtr->setShader(terrainShaderPtr);

	enginePtr->getWorker()->subscribe(terrainRendererPtr);

	// player creation
	{
		mod::player::PlayerPtr playerPtr = std::make_shared<mod::player::Player>(enginePtr, terrainPtr);
		mod::player::PlayerShaderPtr playerShaderPtr = std::make_shared<mod::player::PlayerShader>();

		vd::component::EntityRendererPtr playerRendererPtr = std::make_shared<vd::component::EntityRenderer>();
		playerRendererPtr->setEntity(playerPtr);
		playerRendererPtr->setRenderConfig(ccwConfigPtr);
		playerRendererPtr->setShader(playerShaderPtr);

		enginePtr->getWorker()->subscribe(playerRendererPtr);

		// set camera follows this entity
		cameraInitParameters.entityPtr = playerPtr;

		// with an offset of 1 on Y axis
		// "don't look at shoes, look at chest"
		cameraInitParameters.playerPositionOffset = glm::vec3(0.0f, 1.0f, 0.0f);

		// using this terrain
		cameraInitParameters.terrainPtr = terrainPtr;
	}
	
	// sky creation
	{
        vd::component::SkyConfigPtr skyConfigPtr = std::make_shared<vd::component::SkyConfig>();

		mod::sky::SkyPtr skyPtr = std::make_shared<mod::sky::Sky>(enginePtr);
		mod::sky::SkyShaderPtr skyShaderPtr = std::make_shared<mod::sky::SkyShader>();

		vd::component::EntityRendererPtr skyRendererPtr = std::make_shared<vd::component::EntityRenderer>();
		skyRendererPtr->setEntity(skyPtr);
		skyRendererPtr->setRenderConfig(skyConfigPtr);
		skyRendererPtr->setShader(skyShaderPtr);

		enginePtr->getWorker()->subscribe(skyRendererPtr);

		// object generator
		{
			mod::sobj::StaticObjectPlacerPtr staticObjectPlacerPtr =
			        std::make_shared<mod::sobj::StaticObjectPlacer>(terrainPtr, 7000, 10.0f);
			mod::sobj::StaticObjectShaderPtr staticObjectShaderPtr = std::make_shared<mod::sobj::StaticObjectShader>();

			mod::sobj::StaticObjectRendererPtr staticObjectRendererPtr = std::make_shared<mod::sobj::StaticObjectRenderer>();
			staticObjectRendererPtr->setStaticObjectPlacer(staticObjectPlacerPtr);
			staticObjectRendererPtr->setRenderConfig(ccwConfigPtr);
			staticObjectRendererPtr->setShader(staticObjectShaderPtr);

			enginePtr->getWorker()->subscribe(staticObjectRendererPtr);
		}
	}

	// engine init
	{
		//core::FreeCameraInitParameters cameraInitParameters;
		//cameraInitParameters.initPosition = glm::vec3(0.0f, 1.0f, 2.5f);
		//cameraInitParameters.initTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		//cameraInitParameters.speed = 1.0f;

		// send init signal
		enginePtr->init(&cameraInitParameters);
	}

    /*{
        mod::gui::GuiQuadPtr guiQuadPtr =
                std::make_shared<mod::gui::GuiQuad>(enginePtr,
                                                    enginePtr->getShadowManager()->getShadowTexture(),
                                                    glm::vec2(0.75f, 0.75f),
                                                    glm::vec2(0.250f, 0.250f));
        mod::gui::GuiShaderPtr guiShaderPtr = std::make_shared<mod::gui::GuiShader>();

        mod::gui::GuiRendererPtr  guiRendererPtr = std::make_shared<mod::gui::GuiRenderer>();
        guiRendererPtr->setRenderConfig(ccwConfigPtr);
        guiRendererPtr->setGuiQuad(guiQuadPtr);
        guiRendererPtr->setShader(guiShaderPtr);

        guiRendererPtr->init();

        enginePtr->getWorker()->subscribe(guiRendererPtr);
    }*/

	// start mainloop
	enginePtr->start();

	return 0;
}
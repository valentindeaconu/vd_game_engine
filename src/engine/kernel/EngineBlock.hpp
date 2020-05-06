#ifndef __ENGINE_BLOCK_HPP_
#define __ENGINE_BLOCK_HPP_

#include <engine/foundation/GL.hpp>

#include <thread>
#include <iostream>
#include <string>

#include "EngineWorker.hpp"

#include <engine/core/Window.hpp>

#include <engine/core/impl/EntityCamera.hpp>

#include <engine/config/EngineConfig.hpp>

namespace vd
{
	class Engine
	{
	public:
		Engine();
		
		void setup(int windowWidth, int windowHeight, const char* windowTitle);
		void init(core::CameraInitParametersPtr cameraParameters);
		void start();

		// Get the number of frames per second of the engine
		int getFramesPerSecond() const;

		// Get the time necessary by a frame to be completely rendered
		float getFrameTime() const;

		kernel::EngineWorkerPtr& getWorker();
		const kernel::EngineWorkerPtr& getWorker() const;

		core::InputHandlerPtr& getInputHandler();
		const core::InputHandlerPtr& getInputHandler() const;

		core::WindowPtr& getWindow();
		const core::WindowPtr& getWindow() const;

		core::CameraPtr& getCamera();
		const core::CameraPtr& getCamera() const;

		config::EngineConfigPtr& getEngineConfig();
		const config::EngineConfigPtr& getEngineConfig() const;
	private:
		void run();
		void stop();
		void update();
		void cleanUp();

		int fps; // frames per second
		float ftis; // frame time (in seconds)

		float frametime;
		bool isRunning;

		core::InputHandlerPtr inputHandlerPtr;
		core::WindowPtr windowPtr;

		typedef core::impl::EntityCamera	CameraImpl;
		core::CameraPtr cameraPtr;

		config::EngineConfigPtr configPtr;

		kernel::EngineWorkerPtr engineWorkerPtr;
	};
	typedef std::shared_ptr<Engine>	EnginePtr;
}

#endif // !__ENGINE_BLOCK_HPP_
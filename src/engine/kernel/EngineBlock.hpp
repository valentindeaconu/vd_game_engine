#ifndef __ENGINE_BLOCK_HPP_
#define __ENGINE_BLOCK_HPP_

#include <engine/foundation/GL.hpp>

#include <thread>
#include <iostream>
#include <string>
#include <vector>

#include "EngineWorker.hpp"

#include <engine/core/Window.hpp>

#include <engine/core/impl/EntityCamera.hpp>
#include <engine/core/impl/FreeCamera.hpp>

#include <engine/config/EngineConfig.hpp>

#include <engine/shadow/ShadowManager.hpp>

#include <engine/glmodel/buffer/FrameBuffer.hpp>

#include <engine/config/MetaConfig.hpp>

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
		[[nodiscard]] int getFramesPerSecond() const;

		// Get the time necessary by a frame to be completely rendered
		[[nodiscard]] float getFrameTime() const;

		kernel::EngineWorkerPtr& getWorker();
		[[nodiscard]] const kernel::EngineWorkerPtr& getWorker() const;

		core::InputHandlerPtr& getInputHandler();
		[[nodiscard]] const core::InputHandlerPtr& getInputHandler() const;

		core::WindowPtr& getWindow();
		[[nodiscard]] const core::WindowPtr& getWindow() const;

		core::CameraPtr& getCamera();
		[[nodiscard]] const core::CameraPtr& getCamera() const;

        shadow::ShadowManagerPtr& getShadowManager();
        [[nodiscard]] const shadow::ShadowManagerPtr& getShadowManager() const;

		config::EngineConfigPtr& getEngineConfig();
		[[nodiscard]] const config::EngineConfigPtr& getEngineConfig() const;

		void addRenderingFramebuffer(const buffer::FrameBufferPtr& frameBufferPtr, const config::MetaConfigPtr& configPtr);
	private:
		void run();
		void stop();
		void update();
		void cleanUp();

		int fps; // frames per second
		float frameTimeInSeconds; // frame time (in seconds)

		float frameTime;
		bool isRunning;

		core::InputHandlerPtr inputHandlerPtr;
		core::WindowPtr windowPtr;

		//typedef core::impl::EntityCamera	CameraImpl;
		typedef core::impl::FreeCamera		CameraImpl;
		core::CameraPtr cameraPtr;

		shadow::ShadowManagerPtr shadowManagerPtr;

		config::EngineConfigPtr configPtr;

		kernel::EngineWorkerPtr engineWorkerPtr;

		struct RenderingFrameBuffer {
		    buffer::FrameBufferPtr frameBufferPtr;
		    config::MetaConfigPtr configPtr;
		};

		std::vector<RenderingFrameBuffer> renderingFrameBuffers;
	};
	typedef std::shared_ptr<Engine>	EnginePtr;
}

#endif // !__ENGINE_BLOCK_HPP_
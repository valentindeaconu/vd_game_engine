#include "EngineBlock.hpp"

using namespace std::chrono_literals;

namespace vd
{
	Engine::Engine()
		: isRunning(false)
		, frameTime(1 / 100.0f)
		, fps(0)
	{
	}

	void Engine::setup(int windowWidth, int windowHeight, const char* windowTitle) {
		// InputHandler creation
		inputHandlerPtr = std::make_shared<core::InputHandler>();
		
		// Window creation
		windowPtr = std::make_shared<core::Window>(inputHandlerPtr);
		windowPtr->create(windowWidth, windowHeight, windowTitle);

		// Camera creation
		cameraPtr = std::make_shared<CameraImpl>(inputHandlerPtr);

		// ShadowManager creation
		shadowManagerPtr = std::make_shared<shadow::ShadowManager>();

		// Worker creation
		engineWorkerPtr = std::make_shared<kernel::EngineWorker>();

		// Config creation
		configPtr = std::make_shared<config::EngineConfig>("./resources/engine_settings.cfg");
	}

	void Engine::init(core::CameraInitParametersPtr cameraParameters) {
		// GL init configs
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_FRAMEBUFFER_SRGB);
		glEnable(GL_DEPTH_TEST); // enable depth-testing
		glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
		glEnable(GL_CULL_FACE); // cull face
		glCullFace(GL_BACK); // cull back face
		glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Camera init
		cameraPtr->init(cameraParameters);

		// Config init
		configPtr->parse();

		// Worker init
		engineWorkerPtr->init();

		// Shadow Manager init
		shadowManagerPtr->init(windowPtr,
                         cameraPtr,
                         configPtr->getShadowMapSize(),
                         configPtr->getShadowDistance(),
                         configPtr->getShadowTransitionDistance(),
                         configPtr->getShadowOffset());
	}

	void Engine::start() {
		if (isRunning)
			return;
		run();
	}

	void Engine::run() {
		isRunning = true;

		int frames = 0;
		long frameCounter = 0;

		auto lastTime = std::chrono::high_resolution_clock::now();
		double unprocessedTime = 0;

		// Rendering Loop
		while (isRunning)
		{
			bool renderFrame = false;
			auto startTime = std::chrono::high_resolution_clock::now();
			long passedTime = (startTime - lastTime).count();
			lastTime = startTime;

			unprocessedTime += passedTime / (double)1000000000;
			frameCounter += passedTime;

            frameTimeInSeconds = (float) passedTime / (double)1000000000;

			while (unprocessedTime > frameTime)
			{
				renderFrame = true;
				unprocessedTime -= frameTime;

				if (windowPtr->isCloseRequested())
					stop();

				if (frameCounter >= (double)1000000000)
				{
					fps = frames;
					frames = 0;
					frameCounter = 0;
					std::cout << fps << std::endl;
				}
			}
			if (renderFrame)
			{
				this->update();
				frames++;
			}
			else
			{
				std::this_thread::sleep_for(10ms);
			}
		}

		cleanUp();
	}

	void Engine::stop() {
		if (!isRunning)
			return;

		isRunning = false;
	}

	void Engine::update() {
		windowPtr->update();
		inputHandlerPtr->update();
		cameraPtr->update();

		shadowManagerPtr->update(configPtr->getLights().front());

		// render scene
		glViewport(0, 0, configPtr->getShadowMapSize(), configPtr->getShadowMapSize());
        glDisable(GL_CULL_FACE);
        shadowManagerPtr->bindFramebuffer();
        glClear(GL_DEPTH_BUFFER_BIT);
        this->engineWorkerPtr->update(true);
        shadowManagerPtr->unbindFramebuffer();
        glEnable(GL_CULL_FACE);

        for (const auto& rfb : renderingFrameBuffers) {
            if (rfb.configPtr != nullptr)
                rfb.configPtr->enable();

            rfb.frameBufferPtr->bind();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            this->engineWorkerPtr->update(false);

            rfb.frameBufferPtr->unbind();

            if (rfb.configPtr != nullptr)
                rfb.configPtr->disable();
        }

        glViewport(0, 0, windowPtr->getWidth(), windowPtr->getHeight());
        if (inputHandlerPtr->getKeyDown(GLFW_KEY_M))
        {
            static bool isWireframeMode = false;
            isWireframeMode = !isWireframeMode;

            if (isWireframeMode)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shadowManagerPtr->getShadowTexture()->bind();
		this->engineWorkerPtr->update(false);
	}

	void Engine::cleanUp() {
		windowPtr->dispose();

		engineWorkerPtr->cleanUp();

		shadowManagerPtr->cleanUp();

		glfwTerminate();
	}

	int Engine::getFramesPerSecond() const
	{
		return fps;
	}

	float Engine::getFrameTime() const
	{
		return frameTimeInSeconds;
	}

	kernel::EngineWorkerPtr& Engine::getWorker()
	{
		return engineWorkerPtr;
	}

	const kernel::EngineWorkerPtr& Engine::getWorker() const
	{
		return engineWorkerPtr;
	}

	core::InputHandlerPtr& Engine::getInputHandler()
	{
		return inputHandlerPtr;
	}

	const core::InputHandlerPtr& Engine::getInputHandler() const
	{
		return inputHandlerPtr;
	}

	core::WindowPtr& Engine::getWindow()
	{
		return windowPtr;
	}

	const core::WindowPtr& Engine::getWindow() const
	{
		return windowPtr;
	}

	core::CameraPtr& Engine::getCamera()
	{
		return cameraPtr;
	}

	const core::CameraPtr& Engine::getCamera() const
	{
		return cameraPtr;
	}

    shadow::ShadowManagerPtr& Engine::getShadowManager() {
	    return shadowManagerPtr;
	}

    [[nodiscard]] const shadow::ShadowManagerPtr& Engine::getShadowManager() const {
	    return shadowManagerPtr;
	}

	config::EngineConfigPtr& Engine::getEngineConfig()
	{
		return configPtr;
	}

	const config::EngineConfigPtr& Engine::getEngineConfig() const
	{
		return configPtr;
	}

    void Engine::addRenderingFramebuffer(const buffer::FrameBufferPtr &frameBufferPtr,
                                         const config::MetaConfigPtr &configPtr) {
        this->renderingFrameBuffers.emplace_back();

        RenderingFrameBuffer& rfb = this->renderingFrameBuffers.back();
        rfb.frameBufferPtr = frameBufferPtr;
        rfb.configPtr = configPtr;
    }

}

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
        entityCameraPtr = std::make_shared<core::impl::EntityCamera>(inputHandlerPtr);
		freeCameraPtr = std::make_shared<core::impl::FreeCamera>(inputHandlerPtr);

		// Frustum creation
		frustumPtr = std::make_shared<math::Frustum>(windowPtr, entityCameraPtr);

		// ShadowManager creation
		shadowManagerPtr = std::make_shared<shadow::ShadowManager>();

		// Worker creation
		engineWorkerPtr = std::make_shared<kernel::EngineWorker>();

		// Config creation
		configPtr = std::make_shared<config::EngineConfig>("./resources/engine_settings.cfg");

		// Set shadow framebuffer
        config::MetaConfigPtr shadowConfigPtr = std::make_shared<config::MetaConfig>([]() {
            glDisable(GL_CULL_FACE);
        }, []() {
            glEnable(GL_CULL_FACE);
        });

        this->addRenderingFramebuffer(shadowManagerPtr->getFramebuffer(),
                                      []() { return true; },
                                      shadowConfigPtr,
                                      kernel::RenderingPass::eShadow);
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

		// Camera init
		cameraMode = e3rdPersonCamera;

        vd::core::FreeCameraInitParameters cameraInitParameters = {
                .initPosition = glm::vec3(512.0f, 1.0f, 512.0f),
                .initTarget = glm::vec3(0.0f, 0.0f, 0.0f),
                .speed = 4.0f
        };

		freeCameraPtr->init(&cameraInitParameters);
		entityCameraPtr->init(cameraParameters);

		// Config init
		configPtr->parse();

		// Worker init
		engineWorkerPtr->init();

		// Shadow Manager init
		shadowManagerPtr->init(windowPtr,
                         entityCameraPtr,
                         configPtr->getShadowMapSize(),
                         configPtr->getShadowDistance(),
                         configPtr->getShadowTransitionDistance(),
                         configPtr->getShadowOffset());

		// frustum init
		frustumPtr->init();
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
				this->render();
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
        inputHandlerPtr->update();
        windowPtr->update();

        if (inputHandlerPtr->getKeyDown(GLFW_KEY_C)) {
            if (cameraMode == eFreeCamera) {
                cameraMode = e3rdPersonCamera;
                logger::Logger::log("3rd Person Camera Mode");
            } else {
                cameraMode = eFreeCamera;
                logger::Logger::log("Free Camera Mode");
            }
        }

        if (cameraMode == eFreeCamera) {
            freeCameraPtr->update();
        } else {
            entityCameraPtr->update();
        }

        frustumPtr->update();

        shadowManagerPtr->update(configPtr->getLights().front());

        engineWorkerPtr->update();
	}

	void Engine::render() {
        for (const auto& rfb : renderingFrameBuffers) {
            if (!rfb.preconditionFunc())
                continue;

            if (rfb.configPtr != nullptr) {
                rfb.configPtr->enable();
            }

            rfb.frameBufferPtr->bind();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            this->engineWorkerPtr->render(rfb.renderingPass);

            rfb.frameBufferPtr->unbind();

            if (rfb.configPtr != nullptr)
                rfb.configPtr->disable();
        }

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

        // final render scene
        glViewport(0, 0, windowPtr->getWidth(), windowPtr->getHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->engineWorkerPtr->render(kernel::RenderingPass::eMain);
	}

	void Engine::cleanUp() {
		windowPtr->dispose();

		engineWorkerPtr->cleanUp();

		shadowManagerPtr->cleanUp();

		glfwTerminate();
	}

	int Engine::getFramesPerSecond() const {
		return fps;
	}

	float Engine::getFrameTime() const {
		return frameTimeInSeconds;
	}

	kernel::EngineWorkerPtr& Engine::getWorker() {
		return engineWorkerPtr;
	}

	const kernel::EngineWorkerPtr& Engine::getWorker() const {
		return engineWorkerPtr;
	}

	core::InputHandlerPtr& Engine::getInputHandler() {
		return inputHandlerPtr;
	}

	const core::InputHandlerPtr& Engine::getInputHandler() const {
		return inputHandlerPtr;
	}

	core::WindowPtr& Engine::getWindow() {
		return windowPtr;
	}

	const core::WindowPtr& Engine::getWindow() const {
		return windowPtr;
	}

    const Engine::CameraMode& Engine::getCameraMode() const {
	    return cameraMode;
	}

	core::CameraPtr& Engine::getCamera() {
        if (cameraMode == eFreeCamera)
            return freeCameraPtr;

        return entityCameraPtr;
	}

	const core::CameraPtr& Engine::getCamera() const {
	    if (cameraMode == eFreeCamera)
	        return freeCameraPtr;

	    return entityCameraPtr;
	}

    core::CameraPtr& Engine::getFreeCamera() {
        return freeCameraPtr;
	}

	[[nodiscard]] const core::CameraPtr& Engine::getFreeCamera() const {
        return freeCameraPtr;
	}

    core::CameraPtr& Engine::getEntityCamera() {
        return entityCameraPtr;
	}

    [[nodiscard]] const core::CameraPtr& Engine::getEntityCamera() const {
        return entityCameraPtr;
	}

    shadow::ShadowManagerPtr& Engine::getShadowManager() {
	    return shadowManagerPtr;
	}

    const shadow::ShadowManagerPtr& Engine::getShadowManager() const {
	    return shadowManagerPtr;
	}

	config::EngineConfigPtr& Engine::getEngineConfig() {
		return configPtr;
	}

	const config::EngineConfigPtr& Engine::getEngineConfig() const {
		return configPtr;
	}

    const math::FrustumPtr& Engine::getFrustum() const {
        return frustumPtr;
    }

    const glm::vec4& Engine::getClipPlane() const {
	    return clipPlane;
	}

    void Engine::setClipPlane(const glm::vec4& clipPlane) {
	    this->clipPlane = clipPlane;
	}

    void Engine::addRenderingFramebuffer(const buffer::FrameBufferPtr& frameBufferPtr,
                                         const FramebufferPreconditionFunc& preconditionFunc,
                                         const config::MetaConfigPtr& metaConfigPtr,
                                         const kernel::RenderingPass& renderingPass) {
	    renderingFrameBuffers.push_back({
            .frameBufferPtr = frameBufferPtr,
            .preconditionFunc = preconditionFunc,
            .configPtr = metaConfigPtr,
            .renderingPass = renderingPass
	    });
    }
}

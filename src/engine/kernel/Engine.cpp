#include "Engine.hpp"

using namespace std::chrono_literals;

namespace vd {
    Engine::Engine()
        : m_Running(false)
        , m_FrameTime(1 / 100.0f)
    {
    }

    Engine::~Engine() = default;

    void Engine::Link() {
        m_pWindow = vd::ObjectOfType<window::Window>::Find();
        m_pContext = vd::ObjectOfType<kernel::Context>::Find();
    }

    void Engine::Init() {
        // GL Init configs
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_FRAMEBUFFER_SRGB);
        glEnable(GL_DEPTH_TEST); // enable depth-testing
        glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
        glEnable(GL_CULL_FACE); // cull face
        glCullFace(GL_BACK); // cull back face
        glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

        // Create main rendering pass
        component::RenderingPass renderingPass(
                "Main",
                std::numeric_limits<uint64_t>::max(),
                nullptr,
                vd::g_kEmptyPredicate,
                [&]() { glViewport(0, 0, m_pWindow->Width(), m_pWindow->Height()); }
        );

        this->Add(renderingPass);

        // Initialise all subscribed components
        this->Broadcast(BroadcastType::eInit);
    }

    void Engine::Start() {
        if (m_Running)
            return;

        Run();
    }

    void Engine::Add(const component::RenderingPass& renderingPass) {
        if (m_RenderingPasses.empty() || renderingPass.Priority() < m_RenderingPasses.front().Priority()) {
            m_RenderingPasses.push_front(renderingPass);
        } else if (renderingPass.Priority() >= m_RenderingPasses.back().Priority()) {
            m_RenderingPasses.push_back(renderingPass);
        } else {
            auto it = m_RenderingPasses.begin();
            for (; it != m_RenderingPasses.end(); ++it) {
                if (renderingPass.Priority() < it->Priority()) {
                    break;
                }
            }

            if (it == m_RenderingPasses.end()) {
                throw std::runtime_error("could not insert new rendering pass, unknown state");
            }

            m_RenderingPasses.insert(it, renderingPass);
        }
    }

    void Engine::Remove(const std::string& renderingPassName) {
        auto it = std::find_if(m_RenderingPasses.begin(), m_RenderingPasses.end(), [&](const component::RenderingPass &r) {
            return r.Name() == renderingPassName;
        });

        if (it != m_RenderingPasses.end()) {
            m_RenderingPasses.erase(it);
        }
    }

    void Engine::Run() {
        m_Running = true;

        int frames = 0;
        long frameCounter = 0;

        auto lastTime = std::chrono::high_resolution_clock::now();
        double unprocessedTime = 0;

        // Rendering Loop
        while (m_Running) {
            bool renderFrame = false;
            auto startTime = std::chrono::high_resolution_clock::now();
            long passedTime = (startTime - lastTime).count();
            lastTime = startTime;

            unprocessedTime += passedTime / 1000000000.0;
            frameCounter += passedTime;

            m_pContext->FrameTime() = float(float(passedTime) / 1000000000.0);

            while (unprocessedTime > m_FrameTime) {
                renderFrame = true;
                unprocessedTime -= m_FrameTime;

                if (m_pWindow->CloseRequested())
                    Stop();

                if (frameCounter >= 1000000000.0) {
                    m_pContext->FPS() = frames;
                    std::cout << m_pContext->FPS() << std::endl;

                    frames = 0;
                    frameCounter = 0;
                }
            }
            if (renderFrame) {
                this->Update();
                this->Render();
                frames++;
            } else {
                std::this_thread::sleep_for(10ms);
            }
        }

        CleanUp();
    }

    void Engine::Stop() {
        if (!m_Running)
            return;
        m_Running = false;
    }

    void Engine::Update() {
        this->Broadcast(BroadcastType::eUpdate);
    }

    void Engine::Render() {
        vd::datastruct::Observer::params_t params;

        for (auto& renderingPass : m_RenderingPasses) {
            if (renderingPass.Precondition()) {
                renderingPass.Prepare();

                params["RenderingPass"] = renderingPass.Name();

                if (renderingPass.FrameBuffer() != nullptr)
                    renderingPass.FrameBuffer()->Bind();

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                this->Broadcast(BroadcastType::eRender, params);

                glCheckError();

                if (renderingPass.FrameBuffer() != nullptr)
                    renderingPass.FrameBuffer()->Unbind();

                renderingPass.Finish();
            }
        }
    }

    void Engine::CleanUp() {
        this->Broadcast(BroadcastType::eCleanUp);

        glfwTerminate();
    }
}

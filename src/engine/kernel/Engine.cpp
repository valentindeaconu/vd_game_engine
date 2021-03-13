#include "Engine.hpp"

using namespace std::chrono_literals;

namespace vd {
    Engine::Engine()
        : m_Running(false)
        , m_FrameTime(1 / 100.0f)
    {
    }

    void Engine::Link() {
        m_pWindow = vd::ObjectOfType<window::Window>::Find();
        m_pContext = vd::ObjectOfType<context::Context>::Find();
        m_pThreadPool = vd::ObjectOfType<core::ThreadPool>::Find();
    }

    void Engine::Prepare() {
        auto initJob = m_pThreadPool->CreateJobFor([&]() { Init(); }, "Render");
        initJob->Run();

        m_pUpdateJob = m_pThreadPool->CreateJobFor([&]() { Update(); }, "Update");
        m_pRenderJob = m_pThreadPool->CreateJobFor([&]() { Render(); }, "Render");

        m_pUpdateJob->OnFinish([&]() {
            m_pUpdateJob->Reset();
            m_pThreadPool->PushJobFor(m_pUpdateJob, "Update");
            m_pUpdateJob->Run();
        });

        m_pRenderJob->OnFinish([&]() {
            m_Frames++;
            m_pThreadPool->PushJobFor(m_pRenderJob, "Render");
        });

        initJob->Join();
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

    void Engine::Remove(const std::string& name) {
        auto it = std::find_if(m_RenderingPasses.begin(), m_RenderingPasses.end(), [&](const component::RenderingPass &r) {
            return r.Name() == name;
        });

        if (it != m_RenderingPasses.end()) {
            m_RenderingPasses.erase(it);
            return;
        }
    }

    void Engine::Run() {
        m_Running = true;

        long frameCounter = 0;

        auto lastTime = std::chrono::high_resolution_clock::now();
        double unprocessedTime = 0;

        m_pUpdateJob->Run();
        m_pRenderJob->Run();

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

                if (m_pWindow->CloseRequested()) {
                    Stop();
                    return;
                }

                if (frameCounter >= 1000000000.0) {
                    m_pContext->FPS() = m_Frames.load(std::memory_order_relaxed);

                    m_Frames.store(0, std::memory_order_relaxed);
                    frameCounter = 0;
                }
            }
            if (renderFrame) {
                if (m_pRenderJob->Done()) {
                    m_pRenderJob->Reset();
                    m_pRenderJob->Run();
                }
            } else {
                std::this_thread::sleep_for(10ms);
            }
        }
    }

    void Engine::Stop() {
        if (!m_Running)
            return;
        m_Running = false;

        m_pUpdateJob->OnFinish(vd::g_kEmptyConsumer);
        m_pRenderJob->OnFinish(vd::g_kEmptyConsumer);

        m_pUpdateJob->Run();
        m_pRenderJob->Run();

        auto cleanUpJob = m_pThreadPool->CreateJobFor([&]() { CleanUp(); }, "Render", true);

        m_pThreadPool->Release();
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

        // Create main rendering pass (3d scene rendering pass)
        gl::FrameBufferPtr pSceneFbo = std::make_shared<gl::FrameBuffer>(m_pWindow->Width(), m_pWindow->Height());
        pSceneFbo->Create();
        pSceneFbo->Bind();
        pSceneFbo->PushAttachment(gl::FrameBuffer::eColorTexture, [](gl::Texture2DPtr &pTex) {
            pTex->Bind();
            pTex->LinearFilter();
            pTex->Unbind();
        });
        pSceneFbo->PushAttachment(gl::FrameBuffer::eDepthTexture, [](gl::Texture2DPtr &pTex) {
            pTex->Bind();
            pTex->LinearFilter();
            pTex->Unbind();
        });
        if (pSceneFbo->Status() != gl::FrameBuffer::eComplete) {
            throw RuntimeError("framebuffer is incomplete or has errors");
        }
        pSceneFbo->Unbind();

        m_pContext->SceneFrameBuffer() = pSceneFbo;

        // Main rendering pass will render the scene
        auto beforeFn = [ctx = m_pContext]() {
            if (ctx->WireframeMode()) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
        };
        auto afterFn = [ctx = m_pContext]() {
            if (ctx->WireframeMode()) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        };

        component::RenderingPass rpMain("Main", 100, pSceneFbo, g_kEmptyPredicate, beforeFn, afterFn);
        this->Add(rpMain);

        // Initialise all subscribed components
        this->Broadcast(BroadcastType::eInit);

        Summary();
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
                this->Broadcast(BroadcastType::eRender, params);

                renderingPass.Finish();
            } else {
                renderingPass.FrameBuffer()->Bind();
                renderingPass.FrameBuffer()->Clear();
                renderingPass.FrameBuffer()->Unbind();
            }
        }

#ifdef VDGE_DEBUG
        glCheckError();
#endif
    }

    void Engine::CleanUp() {
        m_pContext->SceneFrameBuffer()->CleanUp();
        service::TextureService::CleanUp();

        this->Broadcast(BroadcastType::eCleanUp);
    }

    void Engine::Summary() {
        std::stringstream ss;

        ss << "Post-Init Summary\n\n";
        ss << "A total of " << m_Observers.size() << " observers subscribed and ";
        ss << m_RenderingPasses.size() << " rendering passes created\n";
        ss << "\n";
        ss << "Rendering passes (in order of drawing):\n";

        int count = 1;
        for (auto it = m_RenderingPasses.begin(); it != m_RenderingPasses.end(); ++it) {
            ss << "\t" << count << ". " << it->Name() << ", priority = " << it->Priority() << "\n";
            ++count;
        }

        Logger::log(ss.str());
    }
}

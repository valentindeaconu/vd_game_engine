//
//	VD GAME ENGINE
//	main.cpp
//

#include <engine/kernel/EngineFactory.hpp>

#include <engine/core/ThreadPool.hpp>

// TODO: Clear main

void TestJobs() {
    vd::core::ThreadPool pool;

    std::vector<std::pair<vd::core::JobPtr, uint64_t*>> jobs;

    for (int i = 0; i < 60; ++i) {
        jobs.emplace_back(std::make_pair(nullptr, nullptr));
        auto& p = jobs.back();

        p.second = new uint64_t;

        vd::Consumer action = [N = i, r = p.second]() {

            uint64_t p = 1;
            for (int k = 0; k < N; ++k) {
                p *= 2;
            }

            *r = p;
        };

        p.first = pool.CreateJob(action);
        p.first->Run();
    }

    for (int i = 0; i < 60; ++i) {
        auto& job = jobs[i].first;
        auto& result = jobs[i].second;

        if (job->Done()) {
            std::stringstream ss;
            ss << "Job #" << i << " done! Result: 2**" << i << "=" << *result << ".";
            vd::Logger::log(ss.str());
        }

        delete result;
    }
}

void SimulateJobsOnEngine() {
    using namespace std::literals::chrono_literals;

    vd::core::ThreadPoolPtr pool = std::make_shared<vd::core::ThreadPool>(std::vector<std::string>({"Update", "Render", "Input"}));

    std::atomic_bool shouldClose(false);

    auto updateAction = [&]() {
        vd::Logger::log("> Update");
    };
    auto updateJob = pool->CreateJobFor(updateAction, "Update");

    auto renderAction = []() {
        vd::Logger::log("> Render");
    };
    auto renderJob = pool->CreateJobFor(renderAction, "Render");

    updateJob->OnFinish([&]() {
        renderJob->Run();
        updateJob->Reset();

        if (!shouldClose.load(std::memory_order_relaxed)) {
            pool->PushJobFor(updateJob, "Update");
        }
    });

    renderJob->OnFinish([&]() {
        renderJob->Reset();

        if (!shouldClose.load(std::memory_order_relaxed)) {
            pool->PushJobFor(renderJob, "Render");
        }
    });

    auto inputAction = [&shouldClose]() {
        std::string s;
        std::cin >> s;
        shouldClose.store(true, std::memory_order_relaxed);
        vd::Logger::warn("Close signal received");
    };
    auto inputJob = pool->CreateJobFor(inputAction, "Input");
    inputJob->Run();

    while (!shouldClose.load(std::memory_order_relaxed)) {
        updateJob->Run();

        std::this_thread::sleep_for(10ms);
    }
    updateJob->Run();

    pool = nullptr;
}

int main(int argc, char ** argv) {
    //TestJobs();
    //SimulateJobsOnEngine();

    /// Engine creation
    vd::EnginePtr pEngine = nullptr;

    try {
        pEngine = vd::EngineFactory::Create();
    } catch (std::exception &e) {
        vd::Logger::terminate("Could not create engine, error: " + std::string(e.what()), 1);
    }

    /// Engine Prepare
    pEngine->Prepare();

    /// Starting Main Loop
    pEngine->Start();

    return 0;
}
//
//	VD GAME ENGINE
//	main.cpp
//

#include <engine/kernel/EngineFactory.hpp>


int main(int argc, char ** argv) {
    /// Engine creation
    vd::EnginePtr pEngine = nullptr;

    try {
        pEngine = vd::EngineFactory::Create();
    } catch (std::exception& e) {
        vd::Logger::terminate("Could not create engine, error: " + std::string(e.what()), 1);
    }

    /// Engine Init
    pEngine->Init();

    /// Starting Main Loop
    pEngine->Start();

    return 0;
}
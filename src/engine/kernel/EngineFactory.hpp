//
// Created by Vali on 11/12/2020.
//

#ifndef VD_GAME_ENGINE_ENGINEFACTORY_HPP
#define VD_GAME_ENGINE_ENGINEFACTORY_HPP

#include "Engine.hpp"

// Requirements
#include <engine/event/EventHandler.hpp>
#include <engine/window/Window.hpp>
#include <engine/camera/Camera.hpp>

// Required managers
#include <engine/culling/FrustumCullingManager.hpp>
#include <engine/light/LightManager.hpp>

// Injection
#include <engine/loader/impl/TinyObjLoaderImpl.hpp>
#include <engine/loader/impl/StbiImpl.hpp>

namespace vd {
    class EngineFactory {
    public:
        static EnginePtr Create();
    private:
        static void Inject();
    };
}


#endif //VD_GAME_ENGINE_ENGINEFACTORY_HPP

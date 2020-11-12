//
// Created by Vali on 11/12/2020.
//

#ifndef VD_GAME_ENGINE_ENGINEFACTORY_HPP
#define VD_GAME_ENGINE_ENGINEFACTORY_HPP

#include "Engine.hpp"

#include <engine/event/EventHandler.hpp>
#include <engine/window/Window.hpp>
#include <engine/camera/ICamera.hpp>

#include <engine/culling/FrustumCullingManager.hpp>
#include <engine/light/LightManager.hpp>

namespace vd {
    class EngineFactory {
    public:
        static EnginePtr Create();
    };
}


#endif //VD_GAME_ENGINE_ENGINEFACTORY_HPP

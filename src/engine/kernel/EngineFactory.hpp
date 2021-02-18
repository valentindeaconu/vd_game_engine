//
// Created by Vali on 11/12/2020.
//

#ifndef VD_GAME_ENGINE_ENGINEFACTORY_HPP
#define VD_GAME_ENGINE_ENGINEFACTORY_HPP

#include "Engine.hpp"

// Injector
#include <engine/injector/CreateAndStore.hpp>
#include <engine/injector/Linker.hpp>

// Requirements
#include <engine/event/EventHandler.hpp>
#include <engine/window/Window.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/kernel/Context.hpp>

// Required managers
#include <engine/culling/FrustumCullingManager.hpp>
#include <engine/light/LightManager.hpp>
#include <engine/fog/FogManager.hpp>

// Injection
#include <engine/loader/impl/TinyObjLoaderImpl.hpp>
#include <engine/loader/impl/StbiImpl.hpp>
#include <engine/loader/impl/StreamImpl.hpp>
#include <engine/loader/impl/VDGEGLSLImpl.hpp>
#include <engine/loader/impl/FreeTypeImpl.hpp>
#include <engine/loader/PropertiesLoader.hpp>

// Modules
#include <modules/ModuleFactory.hpp>

#include <string>

namespace vd {
    class EngineFactory {
    public:
        static EnginePtr Create();
    private:
        static void CreateLoaders();
        static void CreateManagers(vd::EnginePtr& pEngine);
        static void CreateModules(EnginePtr& pEngine);
    };
}


#endif //VD_GAME_ENGINE_ENGINEFACTORY_HPP

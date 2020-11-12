//
// Created by Vali on 11/12/2020.
//

#include "EngineFactory.hpp"

namespace vd {

    EnginePtr EngineFactory::Create() {
        // Engine creation
        vd::EnginePtr enginePtr = std::make_shared<vd::Engine>();

        // Default managers: InputHandler, Window, Camera

        // InputHandler creation
        event::EventHandlerManagerPtr eventHandlerManagerPtr = std::make_shared<event::EventHandlerManager>();
        enginePtr->Subscribe(eventHandlerManagerPtr, 1);

        // Window creation, requirements: InputHandler
        window::WindowManagerPtr windowManagerPtr = std::make_shared<window::WindowManager>();
        enginePtr->Subscribe(windowManagerPtr, 2);

        // Camera creation
        camera::CameraManagerPtr cameraManagerPtr = std::make_shared<camera::CameraManager>();
        enginePtr->Subscribe(cameraManagerPtr, 3);
        vd::ObjectOfType<camera::CameraManager>::Provide(cameraManagerPtr);

        // FrustumCullingManager creation
        culling::FrustumCullingManagerPtr frustumCullingManagerPtr = std::make_shared<culling::FrustumCullingManager>();
        enginePtr->Subscribe(frustumCullingManagerPtr, vd::component::IManager::kDefaultPriority);
        vd::ObjectOfType<culling::FrustumCullingManager>::Provide(frustumCullingManagerPtr);

        // LightManager creation
        light::LightManagerPtr lightManagerPtr = std::make_shared<light::LightManager>();
        enginePtr->Subscribe(lightManagerPtr, vd::component::IManager::kDefaultPriority);
        vd::ObjectOfType<light::LightManager>::Provide(lightManagerPtr);

        enginePtr->Link();

        // Register engine to singleton manager
        vd::ObjectOfType<vd::Engine>::Provide(enginePtr);

        return enginePtr;
    }
}
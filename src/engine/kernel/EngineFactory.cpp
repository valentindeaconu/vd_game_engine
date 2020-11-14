//
// Created by Vali on 11/12/2020.
//

#include "EngineFactory.hpp"

namespace vd {

    EnginePtr EngineFactory::Create() {
        // Engine creation
        EnginePtr enginePtr = std::make_shared<vd::Engine>();

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
        ObjectOfType<camera::CameraManager>::Provide(cameraManagerPtr);

        // FrustumCullingManager creation
        culling::FrustumCullingManagerPtr frustumCullingManagerPtr = std::make_shared<culling::FrustumCullingManager>();
        enginePtr->Subscribe(frustumCullingManagerPtr, vd::component::IManager::kDefaultPriority);
        ObjectOfType<culling::FrustumCullingManager>::Provide(frustumCullingManagerPtr);

        // LightManager creation
        light::LightManagerPtr lightManagerPtr = std::make_shared<light::LightManager>();
        enginePtr->Subscribe(lightManagerPtr, vd::component::IManager::kDefaultPriority);
        ObjectOfType<light::LightManager>::Provide(lightManagerPtr);

        // Injector creation & Injection
        try {
            Inject();
        } catch (std::exception& e) {
            Logger::terminate("Could not inject basic dependencies, error: " + std::string(e.what()), 1);
        }

        enginePtr->Link();

        // Register engine to singleton manager
        ObjectOfType<vd::Engine>::Provide(enginePtr);

        return enginePtr;
    }

    void EngineFactory::Inject() {
        // File Loader
        loader::impl::StreamImplPtr streamImplPtr = std::make_shared<loader::impl::StreamImpl>();
        ObjectOfType<loader::impl::IFileLoader>::Provide(streamImplPtr);

        // Read global properties
        misc::PropertiesPtr propertiesPtr = loader::PropertiesLoader::Load("./resources/global.properties");
        vd::ObjectOfType<vd::misc::Properties>::Provide(propertiesPtr);

        // Object Loader
        loader::impl::TinyObjLoaderImplPtr tinyObjLoaderImplPtr = std::make_shared<loader::impl::TinyObjLoaderImpl>();
        ObjectOfType<loader::impl::IObjectLoader>::Provide(tinyObjLoaderImplPtr);

        // Image Loader
        loader::impl::StbiImplPtr stbiImplPtr = std::make_shared<loader::impl::StbiImpl>();
        ObjectOfType<loader::impl::IImageLoader>::Provide(stbiImplPtr);

        // Shader Loader
        loader::impl::VDGEGLSLImplPtr vdge_glsl_implPtr = std::make_shared<loader::impl::VDGEGLSLImpl>();
        ObjectOfType<loader::impl::IShaderLoader>::Provide(vdge_glsl_implPtr);
    }
}
//
// Created by Vali on 11/12/2020.
//

#include "EngineFactory.hpp"

namespace vd {

    EnginePtr EngineFactory::Create() {
        /// Engine Creation
        EnginePtr pEngine = injector::CreateAndStore<vd::Engine>();

        /// Context Holder
        kernel::ContextPtr pDataStore = injector::CreateAndStore<kernel::Context>();

        /// Linker creation
        injector::LinkerPtr pLinker = injector::CreateAndStore<injector::Linker>();

        /// Loaders
        CreateLoaders();

        /// Included Managers
        CreateManagers(pEngine);

        /// Modules
        CreateModules(pEngine);

        /// Linking Stage
        pEngine->Link();
        pLinker->Link();

        return pEngine;
    }

    void EngineFactory::CreateLoaders() {
        // File Loader
        loader::impl::StreamImplPtr pStreamImpl = std::make_shared<loader::impl::StreamImpl>();
        ObjectOfType<loader::impl::IFileLoader>::Provide(pStreamImpl);

        // Object Loader
        loader::impl::TinyObjLoaderImplPtr pTinyObjLoaderImpl = std::make_shared<loader::impl::TinyObjLoaderImpl>();
        ObjectOfType<loader::impl::IObjectLoader>::Provide(pTinyObjLoaderImpl);

        // Image Loader
        loader::impl::StbiImplPtr pStbiImpl = std::make_shared<loader::impl::StbiImpl>();
        ObjectOfType<loader::impl::IImageLoader>::Provide(pStbiImpl);

        // Shader Loader
        loader::impl::VDGEGLSLImplPtr pVDGEGLSLImpl = std::make_shared<loader::impl::VDGEGLSLImpl>();
        ObjectOfType<loader::impl::IShaderLoader>::Provide(pVDGEGLSLImpl);
    }

    void EngineFactory::CreateManagers(EnginePtr& pEngine) {
        // InputHandler creation
        pEngine->Subscribe(std::make_shared<event::EventHandlerManager>(), 1);

        // Window creation, requirements: InputHandler
        pEngine->Subscribe(std::make_shared<window::WindowManager>(), 2);

        // Camera creation
        pEngine->Subscribe(injector::CreateAndStore<camera::CameraManager>(), 3);

        // FrustumCullingManager creation
        pEngine->Subscribe(injector::CreateAndStore<culling::FrustumCullingManager>(), component::IManager::kDefaultPriority);

        // LightManager creation
        pEngine->Subscribe(injector::CreateAndStore<light::LightManager>("./resources/properties/light.properties"), component::IManager::kDefaultPriority);

        // FogManager creation
        pEngine->Subscribe(injector::CreateAndStore<fog::FogManager>("./resources/properties/fog.properties"), component::IManager::kDefaultPriority);
    }

    void EngineFactory::CreateModules(EnginePtr& pEngine) {
        /// Call ModuleFactory
        std::make_shared<mod::ModuleFactory>()->Create(pEngine);
    }

}
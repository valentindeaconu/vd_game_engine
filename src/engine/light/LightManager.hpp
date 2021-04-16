//
// Created by Vali on 11/10/2020.
//

#ifndef VD_GAME_ENGINE_LIGHTMANAGER_HPP
#define VD_GAME_ENGINE_LIGHTMANAGER_HPP

#include <memory>
#include <vector>

#include <engine/component/IManager.hpp>
#include <engine/component/IShading.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/core/ThreadPool.hpp>

#include <engine/property/Properties.hpp>
#include <engine/loader/PropertiesLoader.hpp>

#include "Light.hpp"

namespace vd::light {
    class LightManager 
        : public vd::component::IManager
        , public vd::component::IShading
        , public vd::injector::Injectable {
    public:
        explicit LightManager(const std::string& propsFilePath);

        void Link() override;

        void Init() override;
        void Update() override;
        void CleanUp() override;

        void AddUniforms(const gl::wrappers::ShaderPtr& pShader) override;
        void SetUniforms(const gl::wrappers::ShaderPtr& pShader) override;

        [[nodiscard]] const LightPtr& Sun() const;
        [[nodiscard]] const std::vector<LightPtr>& Lights() const;
    private:
        LightPtr                m_pSun;
        std::vector<LightPtr>   m_Lights;

        camera::CameraPtr       m_pCamera;
        core::ThreadPoolPtr     m_pThreadPool;
    };
    typedef std::shared_ptr<LightManager>   LightManagerPtr;
}


#endif //VD_GAME_ENGINE_LIGHTMANAGER_HPP

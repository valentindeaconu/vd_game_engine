//
// Created by Vali on 11/25/2020.
//

#ifndef VD_GAME_ENGINE_FOGMANAGER_HPP
#define VD_GAME_ENGINE_FOGMANAGER_HPP

#include <memory>

#include <engine/component/IManager.hpp>
#include <engine/component/IShading.hpp>

#include <engine/property/Properties.hpp>
#include <engine/loader/PropertiesLoader.hpp>

#include "Fog.hpp"

namespace vd::fog {
    class FogManager : public component::IManager, public component::IShading {
    public:
        explicit FogManager(const std::string& propsFilePath);

        void Init() override;
        void Update() override;
        void CleanUp() override;

        void AddUniforms(const gl::wrappers::ShaderPtr& pShader) override;
        void SetUniforms(const gl::wrappers::ShaderPtr& pShader) override;

        void FogColor(const glm::vec3& color);

        [[nodiscard]] const FogPtr& Fog() const;
    private:
        FogPtr m_pFog;
    };
    typedef std::shared_ptr<FogManager> FogManagerPtr;
}


#endif //VD_GAME_ENGINE_FOGMANAGER_HPP

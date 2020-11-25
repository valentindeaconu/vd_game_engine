//
// Created by Vali on 11/10/2020.
//

#ifndef VD_GAME_ENGINE_LIGHTMANAGER_HPP
#define VD_GAME_ENGINE_LIGHTMANAGER_HPP

#include <memory>
#include <vector>

#include <engine/component/IManager.hpp>
#include <engine/component/IShading.hpp>

#include <engine/property/Properties.hpp>
#include <engine/loader/PropertiesLoader.hpp>

#include "Light.hpp"

namespace vd::light {
    class LightManager : public vd::component::IManager, public vd::component::IShading {
    public:
        explicit LightManager(const std::string& propsFilePath);
        ~LightManager();

        void Init() override;
        void Update() override;
        void CleanUp() override;

        void AddUniforms(const gl::ShaderPtr& pShader) override;
        void SetUniforms(const gl::ShaderPtr& pShader) override;

        [[nodiscard]] const LightPtr& Sun() const;
        [[nodiscard]] const std::vector<LightPtr>& Lights() const;
    private:
        LightPtr                m_pSun;
        std::vector<LightPtr>   m_Lights;
    };
    typedef std::shared_ptr<LightManager>   LightManagerPtr;
}


#endif //VD_GAME_ENGINE_LIGHTMANAGER_HPP

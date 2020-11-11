//
// Created by Vali on 11/10/2020.
//

#ifndef VD_GAME_ENGINE_LIGHTMANAGER_HPP
#define VD_GAME_ENGINE_LIGHTMANAGER_HPP

#include <memory>
#include <vector>

#include <engine/component/IManager.hpp>

#include <engine/core/ObjectOfType.hpp>
#include <engine/misc/Properties.hpp>

#include "Light.hpp"

namespace vd::light {
class LightManager : public vd::component::IManager {
    public:
        LightManager();
        ~LightManager();

        void Init() override;
        void Update() override;
        void CleanUp() override;

        const LightPtr& GetSun() const;
        const std::vector<LightPtr>& GetLights() const;
    private:
        LightPtr    m_Sun;
        std::vector<LightPtr>   m_Lights;
    };
    typedef std::shared_ptr<LightManager>   LightManagerPtr;
}


#endif //VD_GAME_ENGINE_LIGHTMANAGER_HPP

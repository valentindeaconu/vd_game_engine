//
// Created by Vali on 9/11/2020.
//

#ifndef VD_GAME_ENGINE_WATER_HPP
#define VD_GAME_ENGINE_WATER_HPP

#include <engine/object/Entity.hpp>

#include "WaterConfig.hpp"

#include <memory>

namespace mod::water {
    class Water : public vd::object::Entity {
    public:
        Water(const vd::EnginePtr& enginePtr, const std::string& configFilePath);
        ~Water();

        void init() override;
        void update() override;
        void cleanUp() override;

        [[nodiscard]] const WaterConfigPtr& getWaterConfig() const;
    private:
        void generatePatch();

        WaterConfigPtr	configPtr;
    };
    typedef std::shared_ptr<Water>  WaterPtr;
}



#endif //VD_GAME_ENGINE_WATER_HPP

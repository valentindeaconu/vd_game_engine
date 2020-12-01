//
// Created by Vali on 11/10/2020.
//

#ifndef VD_GAME_ENGINE_IMANAGER_HPP
#define VD_GAME_ENGINE_IMANAGER_HPP

#include <engine/datastruct/Observer.hpp>

namespace vd::component {
    class IManager : public vd::datastruct::Observer {
    public:
        static const vd::datastruct::Observable::priority_t kDefaultPriority = 100;

        void Render(const vd::datastruct::Observer::params_t& params) override;
    };
}


#endif //VD_GAME_ENGINE_IMANAGER_HPP

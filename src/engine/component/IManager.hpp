//
// Created by Vali on 11/10/2020.
//

#ifndef VD_GAME_ENGINE_IMANAGER_HPP
#define VD_GAME_ENGINE_IMANAGER_HPP

namespace vd::component {
    class IManager {
    public:
        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void CleanUp() = 0;
    };
}


#endif //VD_GAME_ENGINE_IMANAGER_HPP

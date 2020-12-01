//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_INJECTABLE_HPP
#define VD_GAME_ENGINE_INJECTABLE_HPP

#include <memory>

#include "Linker.hpp"

namespace vd::injector {
    class Injectable {
    public:
        Injectable();

        virtual void Link() = 0;
    };
    typedef std::shared_ptr<Injectable>   InjectablePtr;
}


#endif //VD_GAME_ENGINE_INJECTABLE_HPP

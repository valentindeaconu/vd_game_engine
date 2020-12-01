//
// Created by Vali on 11/19/2020.
//

#ifndef VD_GAME_ENGINE_LINKER_HPP
#define VD_GAME_ENGINE_LINKER_HPP

#include <memory>
#include <vector>

#include "ObjectOfType.hpp"

namespace vd::injector {
    class Injectable;

    class Linker {
    public:
        Linker();
        ~Linker();

        void Subscribe(Injectable* injectable);

        void Link();
    private:
        std::vector<Injectable*>    m_Injectables;
    };
    typedef std::shared_ptr<Linker> LinkerPtr;
}


#endif //VD_GAME_ENGINE_LINKER_HPP

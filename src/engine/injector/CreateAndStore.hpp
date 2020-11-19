//
// Created by Vali on 11/19/2020.
//

#ifndef VD_GAME_ENGINE_CREATEANDSTORE_HPP
#define VD_GAME_ENGINE_CREATEANDSTORE_HPP

#include <memory>

#include "ObjectOfType.hpp"

namespace vd::injector {

    template <class T, class... Args>
    std::shared_ptr<T> CreateAndStore(Args&&... args) {
        std::shared_ptr<T> p = std::make_shared<T>(std::forward<Args>(args)...);
        vd::ObjectOfType<T>::Provide(p);

        return p;
    }

}

#endif //VD_GAME_ENGINE_CREATEANDSTORE_HPP

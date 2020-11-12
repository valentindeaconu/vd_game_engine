//
// Created by Vali on 11/12/2020.
//

#include "IManager.hpp"

namespace vd::component {
    void IManager::Render(const vd::datastruct::Observer::params_t& renderingPass) {
        throw std::runtime_error("Managers cannot be rendered");
    }
}

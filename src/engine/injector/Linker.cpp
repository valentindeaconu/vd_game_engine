//
// Created by Vali on 11/19/2020.
//

#include "Linker.hpp"

#include "Injectable.hpp"

namespace vd::injector {

    Linker::Linker() = default;

    Linker::~Linker() = default;

    void Linker::Subscribe(Injectable* injectable) {
        m_Injectables.emplace_back(injectable);
    }

    void Linker::Link() {
        for (Injectable* injectable : m_Injectables) {
            injectable->Link();
        }
    }
}

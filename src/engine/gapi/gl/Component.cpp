//
// Created by Vali on 3/17/2021.
//

#include "Component.hpp"

namespace vd::gl {

    Component::Component() 
        : m_Created(false)
    {
    }

    void Component::Create() {
        if (!m_Created) {
            OnCreate();
            m_Created = true;
        }
    }

    void Component::CleanUp() {
        if (m_Created) {
            OnCleanUp();
            m_Created = false;
        }
    }

    void Component::PassIfCreated() const {
        if (!m_Created) {
            throw RuntimeError("Using an unallocated GL Component");
        }
    }

    void Component::PassIfNotCreated() const {
        if (m_Created) {
            throw RuntimeError("Using an allocated GL Component");
        }
    }

}
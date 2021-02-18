//
// Created by Vali on 2/17/2021.
//

#include "Font.hpp"

namespace vd::model {

    Font::Font() = default;
    Font::~Font() = default;

    std::string& Font::Name() {
        return m_Name;
    }

    std::map<uint8_t, Character>& Font::Characters() {
        return m_Characters;
    }
}


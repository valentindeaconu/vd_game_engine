//
// Created by Vali on 9/22/2020.
//

#include "Biome.hpp"

#include <utility>

namespace mod::terrain {

    Biome::Biome()
        : m_MinimumHeight(0.0f)
        , m_MaximumHeight(0.0f)
        , m_Name()
        , m_Material()
    {
    }

    Biome::Biome(std::string name)
        : m_MinimumHeight(0.0f)
        , m_MaximumHeight(0.0f)
        , m_Name(std::move(name))
        , m_Material()
    {
    }

    Biome::~Biome() = default;

    std::string& Biome::Name() {
        return m_Name;
    }

    float& Biome::MinimumHeight() {
        return m_MinimumHeight;
    }

    float& Biome::MaximumHeight() {
        return m_MaximumHeight;
    }

    vd::model::Material& Biome::Material() {
        return m_Material;
    }

    std::vector<sobj::StaticObjectPtr>& Biome::Objects() {
        return m_Objects;
    }

}
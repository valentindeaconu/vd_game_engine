//
// Created by Vali on 3/26/2021.
//

#include "Biome.hpp"

namespace mod::biomes {

    Biome::Biome()
        : m_MinimumHeight(0.0f)
        , m_MaximumHeight(0.0f)
        , m_MinimumSlope(0.0f)
        , m_MaximumSlope(0.0f)
        , m_Name()
        , m_Material()
    {
    }

    Biome::Biome(std::string name)
        : m_MinimumHeight(0.0f)
        , m_MaximumHeight(0.0f)
        , m_MinimumSlope(0.0f)
        , m_MaximumSlope(0.0f)
        , m_Name(std::move(name))
        , m_Material()
    {
    }

    std::string& Biome::Name() {
        return m_Name;
    }

    float& Biome::MinimumHeight() {
        return m_MinimumHeight;
    }

    float& Biome::MaximumHeight() {
        return m_MaximumHeight;
    }

    float& Biome::MinimumSlope() {
        return m_MinimumSlope;
    }

    float& Biome::MaximumSlope() {
        return m_MaximumSlope;
    }

    vd::model::Material& Biome::Material() {
        return m_Material;
    }

    
    std::vector<vd::object::Entity3DPtr>& Biome::Entities() {
        return m_Entities;
    }
}
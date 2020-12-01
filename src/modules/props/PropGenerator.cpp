//
// Created by Vali on 11/25/2020.
//

#include "PropGenerator.hpp"

namespace mod::props {

    PropGenerator::PropGenerator(int propCount)
        : m_kCount(propCount)
        , m_TerrainSize(0)
        , m_ScaleXZ(0.0f)
    {
    }

    PropGenerator::~PropGenerator() = default;

    void PropGenerator::Link() {
        m_pTerrain = vd::ObjectOfType<terrain::Terrain>::Find();
    }

    void PropGenerator::GenerateLocations() {
        m_ScaleXZ = m_pTerrain->Properties()->Get<float>("ScaleXZ");
        m_TerrainSize = glm::floor(m_ScaleXZ / 2.0f);

        const auto& atlas = m_pTerrain->Biomes();

        std::random_device rd{};
        std::mt19937 gen{ rd() };
        std::uniform_real_distribution<float> d(-m_TerrainSize, m_TerrainSize);

        for (int propIndex = 0; propIndex < m_kCount; ++propIndex) {
            glm::vec2 location = NextLocation(gen, d);

            Placement placement;

            placement.Location.x = location.x;
            placement.Location.z = location.y;
            placement.Location.y = m_pTerrain->HeightAt(location.x, location.y);

            std::vector<PropPtr> propsAtLocation;
            auto biomesAtLocation = m_pTerrain->BiomesAt(location.x, location.y);
            for (auto& biomeAtLocation : biomesAtLocation) {
                auto& props = biomeAtLocation->Props();
                propsAtLocation.insert(propsAtLocation.end(), props.begin(), props.end());
            }

            std::uniform_int_distribution<size_t> d_i(0, propsAtLocation.size() - 1);
            size_t objectRandomIndex = d_i(gen);
            placement.Prop = propsAtLocation[objectRandomIndex];

            m_Placements.emplace_back(std::move(placement));
        }
    }

    const PropGenerator::PlacementVec& PropGenerator::Placements() const {
        return m_Placements;
    }

    bool PropGenerator::OnSurface(float x, float y) const {
        return (x > -m_TerrainSize && y > -m_TerrainSize && x < m_TerrainSize && y < m_TerrainSize);
    }

    glm::vec2 PropGenerator::NextLocation(std::mt19937& gen, std::uniform_real_distribution<float>& d) const {
        glm::vec2 location;

        bool foundSomethingToPlace = false;
        do {
            do { location = glm::vec2(d(gen), d(gen)); } while (!OnSurface(location.x, location.y));

            auto biomesAtLocation = m_pTerrain->BiomesAt(location.x, location.y);

            for (auto& biomeAtLocation : biomesAtLocation) {
                if (!biomeAtLocation->Props().empty()) {
                    foundSomethingToPlace = true;
                    break;
                }
            }
        } while (!foundSomethingToPlace);

        return location;
    }
}
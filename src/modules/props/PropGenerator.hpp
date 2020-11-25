//
// Created by Vali on 11/25/2020.
//

#ifndef VD_GAME_ENGINE_PROPGENERATOR_HPP
#define VD_GAME_ENGINE_PROPGENERATOR_HPP

#include <glm/glm.hpp>
#include <memory>
#include <random>

#include <engine/injector/Injectable.hpp>
#include <modules/terrain/Terrain.hpp>

#include "Prop.hpp"

namespace mod::props {
    class PropGenerator : public vd::injector::Injectable {
    public:
        struct Placement {
            glm::vec3   Location;
            PropPtr     Prop;
        };
        typedef std::vector<Placement>  PlacementVec;

        explicit PropGenerator(int propCount);
        ~PropGenerator();

        void Link() override;

        void GenerateLocations();

        [[nodiscard]] const PlacementVec& Placements() const;
    private:
        [[nodiscard]] bool OnSurface(float x, float y) const;
        glm::vec2 NextLocation(std::mt19937& gen, std::uniform_real_distribution<float>& d) const;

        const int m_kCount;

        float m_TerrainSize;
        float m_ScaleXZ;

        PlacementVec m_Placements;

        terrain::TerrainPtr m_pTerrain;
    };
    typedef std::shared_ptr<PropGenerator>  PropGeneratorPtr;
}


#endif //VD_GAME_ENGINE_PROPGENERATOR_HPP

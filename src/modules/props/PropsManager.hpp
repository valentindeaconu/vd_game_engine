//
// Created by Vali on 3/26/2021.
//

#ifndef VDGE_PROPSMANAGER_HPP
#define VDGE_PROPSMANAGER_HPP

#include <string>
#include <unordered_map>
#include <memory>
#include <random>
#include <glm/glm.hpp>

#include <engine/injector/Injectable.hpp>
#include <modules/biomes/BiomeManager.hpp>
#include <modules/terrain/Terrain.hpp>
#include <modules/water/Water.hpp>

#include <engine/loader/PropertiesLoader.hpp>

#include <engine/component/IManager.hpp>

#include "Prop.hpp"

namespace mod::props {
    class PropsManager : public vd::component::IManager, public vd::injector::Injectable {
    public:
        struct Placement {
            glm::vec3 Location;
            PropPtr   Prop;
        };
        typedef std::vector<Placement>  PlacementVec;

        PropsManager(const std::string& propsFilePath);
        
        void Link() override;

        void Init() override;
        void Update() override;
        void CleanUp() override;

        [[nodiscard]] uint64_t SpawnableProps() const;
        
        [[nodiscard]] PropPtr PropByName(const std::string& name) const;

        [[nodiscard]] const PlacementVec& Placements() const;
    private:
        void GenerateLocations();
        
        uint64_t m_SpawnableProps;

        std::unordered_map<std::string, PropPtr>    m_Props;

        PlacementVec m_Placements;

        mod::biomes::BiomeManagerPtr    m_pBiomeManager;
        mod::terrain::TerrainPtr        m_pTerrain;
        mod::water::WaterPtr            m_pWater;
    };
    typedef std::shared_ptr<PropsManager>   PropsManagerPtr;
}

#endif //VDGE_PROPSMANAGER_HPP
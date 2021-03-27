//
// Created by Vali on 3/26/2021.
//

#include "PropsManager.hpp"

namespace mod::props {

    PropsManager::PropsManager(const std::string& propsFilePath) {
        auto pProps = vd::loader::PropertiesLoader::Load(propsFilePath);

        for (int i = 0; ; ++i) {
            try {
                m_SpawnableProps = pProps->Get<int>("SpawnableProps");

               const std::string prefix = "Prop." + std::to_string(i);

               const auto kName = pProps->Get<std::string>(prefix + ".Name");
               const auto kScale = pProps->Get<float>(prefix + ".Scale");
               const auto kUnderWater = pProps->Get<bool>(prefix + ".UnderWater");

               std::vector<props::Prop::Details> propsDetails;
               for (int j = 0; ; ++j) {
                   try {
                       const std::string lodObjectPrefix = prefix + ".Level." + std::to_string(j);

                       props::Prop::Details propDetails;
                       propDetails.Path = pProps->Get<std::string>(lodObjectPrefix + ".Location");
                       propDetails.File = pProps->Get<std::string>(lodObjectPrefix + ".File");
                       propDetails.Distance = pProps->Get<float>(lodObjectPrefix + ".Distance");
                       propDetails.Billboard = pProps->Get<bool>(lodObjectPrefix + ".Billboard");

                       propsDetails.emplace_back(std::move(propDetails));
                   } catch (std::invalid_argument& e) {
                       break;
                   }
               }

                props::PropPtr pProp = std::make_shared<props::Prop>(propsDetails);
                pProp->WorldTransform().Scale() = glm::vec3(kScale, kScale, kScale);

                // TODO: Using the unused local transform to keep information about spawning this prop under water
                //  Maybe using another component will be a better approach
                pProp->LocalTransform().Translation() = glm::vec3(kUnderWater ? -1.0f : 1.0f);

                m_Props[kName] = std::move(pProp);
            } catch (std::invalid_argument& e) {
                break;
            }
        }
    }
    
    void PropsManager::Link() {
        m_pBiomeManager = vd::ObjectOfType<mod::biomes::BiomeManager>::Find();
        m_pTerrain = vd::ObjectOfType<mod::terrain::Terrain>::Find();
        m_pWater = vd::ObjectOfType<mod::water::Water>::Find();
    }

    void PropsManager::Init() {
        std::unordered_map<std::string, vd::object::Entity3DPtr> base;

        for (auto& p : m_Props) {
            p.second->Init();

            base[p.first] = std::dynamic_pointer_cast<vd::object::Entity3D>(p.second);
        }

        m_pBiomeManager->Populate(base);

        GenerateLocations();
    }

    void PropsManager::Update() {
        // Prop::Update is an empty method because props are static objects and never updates
    }
    
    void PropsManager::CleanUp() {
        for (auto& p : m_Props) {
            p.second->CleanUp();
        }
    }
    
    uint64_t PropsManager::SpawnableProps() const {
        return m_SpawnableProps;
    }

    PropPtr PropsManager::PropByName(const std::string& name) const {
        auto it = m_Props.find(name);
        if (it != m_Props.end()) {
            return it->second;
        }

        return nullptr;
    }

    const PropsManager::PlacementVec& PropsManager::Placements() const {
        return m_Placements;
    }

    void PropsManager::GenerateLocations() {
        const float mapSize = glm::abs(m_pTerrain->WorldTransform().Translation().x);

        std::random_device rd{};
        std::mt19937 gen{ rd() };
        std::uniform_real_distribution<float> d(-mapSize, mapSize);

        for (int propIndex = 0; propIndex < m_SpawnableProps; ++propIndex) {
            glm::vec3 location;

            std::vector<PropPtr> propsAtLocation;

            // Search until a valid location with available props is found
            do {
                propsAtLocation.clear();

                // Generate random locations until a valid location is found (within the map's bounds)
                do { 
                    location = glm::vec3(d(gen), 0.0f, d(gen)); 
                } while (!ValidLocation(location.x, location.y, mapSize));

                // Compute height
                location.y = m_pTerrain->HeightAt(location.x, location.z);

                // Check if the location is under water
                bool isUnderWater = (location.y < m_pWater->GetHeight());

                // Get all biomes at the generated location
                biomes::BiomePtrVec biomesAtLocation = m_pBiomeManager->BiomesAt(location.x, location.z);

                // for each entity in each biome, if it is a prop check if it can live
                for (auto& biomeAtLocation : biomesAtLocation) {
                    for (auto& entity : biomeAtLocation->Entities()) {
                        // A prop can live anywhere on the land, but some can live underwater
                        if (!isUnderWater || (isUnderWater && entity->LocalTransform().Translation().x < 0.0f)) {
                            try {
                                PropPtr prop = std::dynamic_pointer_cast<Prop>(entity);
                                propsAtLocation.emplace_back(std::move(prop));
                            } catch (std::exception& e) { 
                                // if object found is not a prop, ignore it
                            }
                        }    
                    }
                }
            } while (propsAtLocation.empty());

            // pick a random object from the available ones
            std::uniform_int_distribution<size_t> d_i(0, propsAtLocation.size() - 1);
            size_t objectRandomIndex = d_i(gen);

            // Create placement structure
            Placement placement = {
                .Location = location,
                .Prop = propsAtLocation[objectRandomIndex]
            };

            m_Placements.emplace_back(std::move(placement));
        }
    }

    bool PropsManager::ValidLocation(float x, float y, float mapSize) const {
        if (x > -mapSize && y > -mapSize && x < mapSize && y < mapSize) {
            float dist = glm::length(glm::vec2(x, y) - m_pTerrain->Center());
            return dist < m_pTerrain->Radius().x;
        }

        return false;
    }

    bool PropsManager::PropUnderWater(const std::string& name) const {
        auto it = m_UnderWater.find(name);
        if (it != m_UnderWater.end()) {
            return it->second;
        }

        return false;
    }

}
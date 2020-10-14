//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAIN_HPP
#define VD_GAME_ENGINE_TERRAIN_HPP

#include <engine/object/Entity.hpp>

#include <engine/foundation/algorithm//TreeHelper.hpp>

#include <memory>
#include <vector>

#include "TerrainConfig.hpp"
#include "TerrainNode.hpp"

namespace mod::terrain {
    class Terrain : public vd::object::Entity {
    public:
        Terrain(const vd::EnginePtr& enginePtr, const std::string& configFilePath);
        ~Terrain();

        void init() override;
        void update() override;
        void cleanUp() override;

        [[nodiscard]] const TerrainConfigPtr& GetTerrainConfig() const;

        [[nodiscard]] const TerrainNode::ptr_type_t& GetRootNode() const;

    private:
        void generatePatch();

        TerrainConfigPtr m_ConfigPtr;

        TerrainNode::ptr_type_t m_RootNode;
    };
    typedef std::shared_ptr<Terrain>    TerrainPtr;
}


#endif //VD_GAME_ENGINE_TERRAIN_HPP

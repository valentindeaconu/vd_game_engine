//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAINRENDERER_HPP
#define VD_GAME_ENGINE_TERRAINRENDERER_HPP

#include <engine/component/Renderer.hpp>

#include "Terrain.hpp"
#include "TerrainNode.hpp"

namespace vd::terrain {
    class TerrainRenderer : public component::Renderer {
    public:
        TerrainRenderer();
        ~TerrainRenderer();

        void init() override;
        void update() override;
        void render(const vd::kernel::RenderingPass& renderingPass) override;
        void cleanUp() override;

        TerrainPtr& getTerrain();
        [[nodiscard]] const TerrainPtr& getTerrain() const;
        void setTerrain(const TerrainPtr& terrainPtr);

    private:
        void renderNode(const TerrainNodePtr& nodePtr);

        bool isReady() override;

        TerrainPtr terrainPtr;
    };
    typedef std::shared_ptr<TerrainRenderer>	TerrainRendererPtr;
}


#endif //VD_GAME_ENGINE_TERRAINRENDERER_HPP

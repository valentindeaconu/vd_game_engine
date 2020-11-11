//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAINRENDERER_HPP
#define VD_GAME_ENGINE_TERRAINRENDERER_HPP

#include <engine/component/Renderer.hpp>

#include "Terrain.hpp"
#include "TerrainNode.hpp"

namespace mod::terrain {
    class TerrainRenderer : public vd::component::Renderer {
    public:
        TerrainRenderer();
        ~TerrainRenderer();

        void Init() override;
        void Update() override;
        void Render(const vd::kernel::RenderingPass& renderingPass) override;
        void CleanUp() override;

        TerrainPtr& GetTerrain();
        [[nodiscard]] const TerrainPtr& GetTerrain() const;
        void SetTerrain(const TerrainPtr& terrainPtr);

    private:
        void renderNode(const TerrainNode::ptr_type_t& nodePtr);

        bool IsReady() override;

        TerrainPtr m_TerrainPtr;
    };
    typedef std::shared_ptr<TerrainRenderer>	TerrainRendererPtr;
}


#endif //VD_GAME_ENGINE_TERRAINRENDERER_HPP

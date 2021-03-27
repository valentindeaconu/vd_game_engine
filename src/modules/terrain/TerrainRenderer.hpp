//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAINRENDERER_HPP
#define VD_GAME_ENGINE_TERRAINRENDERER_HPP

#include <engine/component/IRenderer.hpp>

#include <engine/collision/Detector.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/culling/FrustumCullingManager.hpp>

#include "Terrain.hpp"
#include "TerrainNode.hpp"

namespace mod::terrain {
    class TerrainRenderer
            : public vd::component::IRenderer
            , public vd::injector::Injectable {
    public:
        static const vd::datastruct::Observable::priority_t kPriority = kDefaultPriority - 10;

        TerrainRenderer(TerrainPtr terrainPtr,
                        vd::component::IEntityShaderPtr shaderPtr,
                        vd::Consumer beforeExecution = vd::g_kEmptyConsumer,
                        vd::Consumer afterExecution = vd::g_kEmptyConsumer);

        void Link() override;

        void Init() override;
        void Update() override;
        void Render(const params_t& params) override;
        void CleanUp() override;
    private:
        void RenderNode(const TerrainNode::ptr_type_t& pNode);

        bool IsReady() override;

        TerrainPtr m_pTerrain;

        vd::culling::FrustumCullingManagerPtr m_pFrustumCullingManager;
    };
    typedef std::shared_ptr<TerrainRenderer>	TerrainRendererPtr;
}


#endif //VD_GAME_ENGINE_TERRAINRENDERER_HPP

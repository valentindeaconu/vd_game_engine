//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAINRENDERER_HPP
#define VD_GAME_ENGINE_TERRAINRENDERER_HPP

#include <engine/component/IRenderer.hpp>

#include <modules/shadow/ShadowShader.hpp>

#include "Terrain.hpp"
#include "TerrainNode.hpp"

namespace mod::terrain {
    class TerrainRenderer : public vd::component::IRenderer {
    public:
        TerrainRenderer(TerrainPtr terrainPtr,
                        vd::gl::ShaderPtr shaderPtr,
                        vd::Consumer beforeExecution = vd::g_kEmptyConsumer,
                        vd::Consumer afterExecution = vd::g_kEmptyConsumer);
        ~TerrainRenderer();

        void Init() override;
        void Update() override;
        void Render(const params_t& params) override;
        void CleanUp() override;
    private:
        void RenderNode(const TerrainNode::ptr_type_t& nodePtr);

        bool IsReady() override;

        TerrainPtr m_pTerrain;
    };
    typedef std::shared_ptr<TerrainRenderer>	TerrainRendererPtr;
}


#endif //VD_GAME_ENGINE_TERRAINRENDERER_HPP

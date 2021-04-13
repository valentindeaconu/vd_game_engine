//
// Created by Vali on 9/11/2020.
//

#ifndef VD_GAME_ENGINE_WATERRENDERER_HPP
#define VD_GAME_ENGINE_WATERRENDERER_HPP

#include <engine/component/IRenderer.hpp>
#include <engine/component/IEntityShader.hpp>

#include "Water.hpp"

namespace mod::water {
    class WaterRenderer : public vd::component::IRenderer {
    public:
        static const int kPriority = kDefaultPriority + 50;

        WaterRenderer(WaterPtr water, vd::component::IEntityShaderPtr shader);

        void OnInit() override;
        void OnUpdate() override;
        void OnRender(const params_t& params) override;
        void OnCleanUp() override;

        WaterPtr& Water();

    private:
        bool Precondition(const params_t& params) override;
        void Prepare() override;
        void Finish() override;

        WaterPtr                        m_pWater;
        vd::component::IEntityShaderPtr m_pShader;
    };
    typedef std::shared_ptr<WaterRenderer>  WaterRendererPtr;
}

#endif //VD_GAME_ENGINE_WATERRENDERER_HPP

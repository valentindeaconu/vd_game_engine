//
// Created by Vali on 9/11/2020.
//

#ifndef VD_GAME_ENGINE_WATERRENDERER_HPP
#define VD_GAME_ENGINE_WATERRENDERER_HPP

#include <engine/component/IRenderer.hpp>

#include "Water.hpp"

namespace mod::water {
    class WaterRenderer : public vd::component::IRenderer {
    public:
        static const int kPriority = kDefaultPriority + 50;

        WaterRenderer(WaterPtr waterPtr,
                      vd::component::EntityShaderPtr shaderPtr,
                      vd::Consumer beforeExecution = vd::g_kEmptyConsumer,
                      vd::Consumer afterExecution = vd::g_kEmptyConsumer);
        ~WaterRenderer();

        void Init() override;
        void Update() override;
        void Render(const params_t& params) override;
        void CleanUp() override;

        WaterPtr& Water();

    private:
        bool IsReady() override;

        WaterPtr m_pWater;
    };
    typedef std::shared_ptr<WaterRenderer>  WaterRendererPtr;
}

#endif //VD_GAME_ENGINE_WATERRENDERER_HPP

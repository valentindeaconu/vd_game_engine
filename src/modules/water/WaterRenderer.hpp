//
// Created by Vali on 9/11/2020.
//

#ifndef VD_GAME_ENGINE_WATERRENDERER_HPP
#define VD_GAME_ENGINE_WATERRENDERER_HPP

#include <engine/component/Renderer.hpp>

#include "Water.hpp"

namespace mod::water {
    class WaterRenderer : public vd::component::Renderer {
    public:
        WaterRenderer();
        ~WaterRenderer();

        void Init() override;
        void Update() override;
        void Render(const vd::kernel::RenderingPass& renderingPass) override;
        void CleanUp() override;

        WaterPtr& GetWater();
        [[nodiscard]] const WaterPtr& GetWater() const;
        void SetWater(const WaterPtr& waterPtr);

    private:
        bool IsReady() override;

        WaterPtr m_WaterPtr;
    };
    typedef std::shared_ptr<WaterRenderer>  WaterRendererPtr;
}

#endif //VD_GAME_ENGINE_WATERRENDERER_HPP

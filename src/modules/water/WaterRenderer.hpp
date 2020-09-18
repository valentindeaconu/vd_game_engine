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

        void init() override;
        void update() override;
        void render(const vd::kernel::RenderingPass& renderingPass) override;
        void cleanUp() override;

        WaterPtr& getWater();
        [[nodiscard]] const WaterPtr& getWater() const;
        void setWater(const WaterPtr& waterPtr);

    private:
        bool isReady() override;

        WaterPtr waterPtr;
    };
    typedef std::shared_ptr<WaterRenderer>  WaterRendererPtr;
}

#endif //VD_GAME_ENGINE_WATERRENDERER_HPP

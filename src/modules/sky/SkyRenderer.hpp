#ifndef __SKY_RENDERER_HPP_
#define __SKY_RENDERER_HPP_

#include <engine/component/Renderer.hpp>

#include <string>

#include "Sky.hpp"

namespace mod::sky {
    class SkyRenderer : public vd::component::Renderer {
    public:
        SkyRenderer();
        ~SkyRenderer();

        void init() override;
        void update() override;
        void render(const vd::kernel::RenderingPass& renderingPass) override;
        void cleanUp() override;

        SkyPtr& getSky();
        [[nodiscard]] const SkyPtr& getSky() const;
        void setSky(const SkyPtr& skyPtr);

    private:
        bool isReady() override;

        SkyPtr skyPtr;
    };

    typedef std::shared_ptr<SkyRenderer>	SkyRendererPtr;
}

#endif // !__SKY_RENDERER_HPP_
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

        void Init() override;
        void Update() override;
        void Render(const vd::kernel::RenderingPass& renderingPass) override;
        void CleanUp() override;

        SkyPtr& GetSky();
        [[nodiscard]] const SkyPtr& GetSky() const;
        void SetSky(const SkyPtr& skyPtr);

    private:
        bool IsReady() override;

        SkyPtr m_SkyPtr;
    };

    typedef std::shared_ptr<SkyRenderer>	SkyRendererPtr;
}

#endif // !__SKY_RENDERER_HPP_
#ifndef __SKY_RENDERER_HPP_
#define __SKY_RENDERER_HPP_

#include <engine/component/IRenderer.hpp>

#include <engine/shadow/ShadowShader.hpp>

#include <string>

#include "Sky.hpp"

namespace mod::sky {
    class SkyRenderer : public vd::component::IRenderer {
    public:
        SkyRenderer(SkyPtr skyPtr,
                    vd::shader::ShaderPtr shaderPtr,
                    vd::Consumer beforeExecution = vd::g_kEmptyConsumer,
                    vd::Consumer afterExecution = vd::g_kEmptyConsumer);
        ~SkyRenderer();

        void Init() override;
        void Update() override;
        void Render(const params_t& params) override;
        void CleanUp() override;

    private:
        bool IsReady() override;

        SkyPtr m_SkyPtr;
    };

    typedef std::shared_ptr<SkyRenderer>	SkyRendererPtr;
}

#endif // !__SKY_RENDERER_HPP_
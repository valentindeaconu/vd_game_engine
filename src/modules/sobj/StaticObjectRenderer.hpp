#ifndef __STATIC_OBJECT_RENDERER_HPP_
#define __STATIC_OBJECT_RENDERER_HPP_

#include <engine/component/IRenderer.hpp>

#include <engine/shadow/ShadowShader.hpp>

#include <engine/collision/Detector.hpp>

#include <engine/culling/FrustumCullingManager.hpp>

#include <modules/terrain/Terrain.hpp>

#include "StaticObjectPlacer.hpp"

namespace mod::sobj {
    class StaticObjectRenderer : public vd::component::IRenderer {
    public:
        StaticObjectRenderer(StaticObjectPlacerPtr staticObjectPlacerPtr,
                             vd::shader::ShaderPtr shaderPtr,
                             vd::Consumer beforeExecution = vd::g_kEmptyConsumer,
                             vd::Consumer afterExecution = vd::g_kEmptyConsumer);
        ~StaticObjectRenderer();

        void Init() override;
        void Update() override;
        void Render(const params_t& params) override;
        void CleanUp() override;
    private:
        bool IsReady() override;

        StaticObjectPlacerPtr m_StaticObjectPlacerPtr;

        vd::culling::FrustumCullingManagerPtr m_FrustumCullingManagerPtr;
    };
    typedef std::shared_ptr<StaticObjectRenderer>	StaticObjectRendererPtr;
}

#endif // !__STATIC_OBJECT_RENDERER_HPP_

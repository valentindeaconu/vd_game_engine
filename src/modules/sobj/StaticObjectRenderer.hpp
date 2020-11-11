#ifndef __STATIC_OBJECT_RENDERER_HPP_
#define __STATIC_OBJECT_RENDERER_HPP_

#include <engine/component/Renderer.hpp>
#include <engine/shader/Shader.hpp>

#include <engine/collision/Detector.hpp>

#include <engine/culling/FrustumCullingManager.hpp>
#include <engine/core/ObjectOfType.hpp>

#include <modules/terrain/Terrain.hpp>

#include <engine/logger/Logger.hpp>

#include "StaticObjectPlacer.hpp"

namespace mod::sobj {
    class StaticObjectRenderer : public vd::component::Renderer {
    public:
        StaticObjectRenderer();
        ~StaticObjectRenderer();

        void Init() override;
        void Update() override;
        void Render(const vd::kernel::RenderingPass& renderingPass) override;
        void CleanUp() override;

        StaticObjectPlacerPtr& GetStaticObjectPlacer();
        [[nodiscard]] const StaticObjectPlacerPtr& GetStaticObjectPlacer() const;
        void SetStaticObjectPlacer(const StaticObjectPlacerPtr& staticObjectPlacerPtr);

    private:
        bool IsReady() override;

        StaticObjectPlacerPtr m_StaticObjectPlacerPtr;

        vd::culling::FrustumCullingManagerPtr m_FrustumCullingManagerPtr;
    };
    typedef std::shared_ptr<StaticObjectRenderer>	StaticObjectRendererPtr;
}

#endif // !__STATIC_OBJECT_RENDERER_HPP_

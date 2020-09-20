#ifndef __STATIC_OBJECT_RENDERER_HPP_
#define __STATIC_OBJECT_RENDERER_HPP_

#include <engine/component/Renderer.hpp>
#include <engine/shader/Shader.hpp>

#include <modules/terrain/Terrain.hpp>

#include <engine/logger/Logger.hpp>

#include "StaticObjectPlacer.hpp"

namespace mod::sobj
{
    class StaticObjectRenderer : public vd::component::Renderer
    {
    public:
        StaticObjectRenderer();
        ~StaticObjectRenderer();

        void init() override;
        void update() override;
        void render(const vd::kernel::RenderingPass& renderingPass) override;
        void cleanUp() override;

        StaticObjectPlacerPtr& getStaticObjectPlacer();
        [[nodiscard]] const StaticObjectPlacerPtr& getStaticObjectPlacer() const;
        void setStaticObjectPlacer(const StaticObjectPlacerPtr& staticObjectPlacerPtr);

    private:
        bool isReady() override;

        StaticObjectPlacerPtr staticObjectPlacerPtr;
    };
    typedef std::shared_ptr<StaticObjectRenderer>	StaticObjectRendererPtr;
}

#endif // !__STATIC_OBJECT_RENDERER_HPP_

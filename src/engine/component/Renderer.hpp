#ifndef __RENDERER_HPP_
#define __RENDERER_HPP_

#include <string>

#include <engine/object/Entity.hpp>
#include <engine/shader/Shader.hpp>

#include <engine/kernel/EngineWorker.hpp>

#include "RenderConfig.hpp"

namespace vd::component
{
    class Renderer : public kernel::Observer
    {
    public:
        Renderer();
        ~Renderer();

        virtual void init() = 0;
        virtual void update() = 0;
        virtual void cleanUp() = 0;

        RenderConfigPtr& getRenderConfig();
        const RenderConfigPtr& getRenderConfig() const;
        void setRenderConfig(const RenderConfigPtr& renderConfigPtr);

        vd::shader::ShaderPtr& getShader();
        const vd::shader::ShaderPtr& getShader() const;
        void setShader(const vd::shader::ShaderPtr& shaderPtr);
    protected:
        virtual bool isReady();

        RenderConfigPtr renderConfigPtr;
        vd::shader::ShaderPtr shaderPtr;
    };
    typedef std::shared_ptr<Renderer>	RendererPtr;
}

#endif // !__RENDERER_HPP_
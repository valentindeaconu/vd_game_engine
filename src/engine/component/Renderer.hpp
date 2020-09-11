#ifndef __RENDERER_HPP_
#define __RENDERER_HPP_

#include <string>

#include <engine/object/Entity.hpp>
#include <engine/shader/Shader.hpp>

#include <engine/kernel/EngineWorker.hpp>

#include <engine/shadow/ShadowShader.hpp>

#include "RenderConfig.hpp"

namespace vd::component
{
    class Renderer : public kernel::Observer
    {
    public:
        Renderer();
        ~Renderer();

        virtual void init() = 0;
        virtual void update(bool shadowUpdate) = 0;
        virtual void cleanUp() = 0;

        RenderConfigPtr& getRenderConfig();
        [[nodiscard]] const RenderConfigPtr& getRenderConfig() const;
        void setRenderConfig(const RenderConfigPtr& renderConfigPtr);

        shader::ShaderPtr& getShader();
        [[nodiscard]] const shader::ShaderPtr& getShader() const;
        void setShader(const vd::shader::ShaderPtr& shaderPtr);

    protected:
        virtual bool isReady();

        [[nodiscard]] shadow::ShadowShaderPtr& getShadowShader() const;

        RenderConfigPtr renderConfigPtr;
        shader::ShaderPtr shaderPtr;
    };
    typedef std::shared_ptr<Renderer>	RendererPtr;
}

#endif // !__RENDERER_HPP_
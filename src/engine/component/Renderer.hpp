#ifndef __RENDERER_HPP_
#define __RENDERER_HPP_

#include <string>

#include <engine/object/Entity.hpp>
#include <engine/shader/Shader.hpp>
#include <engine/config/MetaConfig.hpp>

#include <engine/kernel/EngineWorker.hpp>

#include <engine/shadow/ShadowShader.hpp>

namespace vd::component {
    class Renderer : public kernel::Observer {
    public:
        Renderer();
        ~Renderer();

        config::MetaConfigPtr& GetRenderConfig();
        [[nodiscard]] const config::MetaConfigPtr& GetRenderConfig() const;
        void SetRenderConfig(const config::MetaConfigPtr& renderConfigPtr);

        shader::ShaderPtr& GetShader();
        [[nodiscard]] const shader::ShaderPtr& GetShader() const;
        void SetShader(const vd::shader::ShaderPtr& shaderPtr);

    protected:
        virtual bool IsReady();

        [[nodiscard]] shadow::ShadowShaderPtr& GetShadowShader() const;

        config::MetaConfigPtr m_ConfigPtr;
        shader::ShaderPtr m_ShaderPtr;
    };
    typedef std::shared_ptr<Renderer>	RendererPtr;
}

#endif // !__RENDERER_HPP_
#ifndef __RENDERER_HPP_
#define __RENDERER_HPP_

#include <engine/datastruct/Observer.hpp>
#include <engine/shader/Shader.hpp>

#include <engine/misc/Types.hpp>

#include <string>

namespace vd::component {
    class IRenderer : public datastruct::Observer {
    public:
        static const vd::datastruct::Observable::priority_t kDefaultPriority = 200;

        explicit IRenderer(shader::ShaderPtr shaderPtr,
                  Consumer beforeExecution = g_kEmptyConsumer,
                  Consumer afterExecution = g_kEmptyConsumer);
        ~IRenderer();

        void Prepare();
        void Finish();

        shader::ShaderPtr& Shader();

    protected:
        virtual bool IsReady();

        vd::Consumer m_BeforeExecution;
        vd::Consumer m_AfterExecution;

        shader::ShaderPtr m_ShaderPtr;
    };
    typedef std::shared_ptr<IRenderer>	RendererPtr;
}

#endif // !__RENDERER_HPP_
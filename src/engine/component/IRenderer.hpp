//
// Created by Vali on 11/10/2020.
//

#ifndef VD_GAME_ENGINE_IRENDERER_HPP
#define VD_GAME_ENGINE_IRENDERER_HPP

#include <engine/datastruct/Observer.hpp>
#include <engine/misc/Types.hpp>

#include <string>

#include "IEntityShader.hpp"

namespace vd::component {
    class IRenderer : public datastruct::Observer {
    public:
        static const vd::datastruct::Observable::priority_t kDefaultPriority = 200;

        explicit IRenderer(IEntityShaderPtr shaderPtr,
                  Consumer beforeExecution = g_kEmptyConsumer,
                  Consumer afterExecution = g_kEmptyConsumer);
        ~IRenderer();

        void Prepare();
        void Finish();

        IEntityShaderPtr& Shader();
    protected:
        virtual bool IsReady();

        vd::Consumer m_BeforeExecution;
        vd::Consumer m_AfterExecution;

        IEntityShaderPtr m_pShader;
    };
    typedef std::shared_ptr<IRenderer>	RendererPtr;
}

#endif // VD_GAME_ENGINE_IRENDERER_HPP
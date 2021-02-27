//
// Created by Vali on 2/19/2021.
//

#ifndef VDGE_RENDERINGEFFECT_HPP
#define VDGE_RENDERINGEFFECT_HPP

#include <memory>

#include <engine/api/gl/FrameBuffer.hpp>

namespace vd::component {
    class IRenderingEffect {
    public:
        explicit IRenderingEffect(std::string name);

        virtual void Init(vd::Dimension dimension) = 0;

        virtual bool Precondition() = 0;
        virtual void Prepare() = 0;
        virtual void Finish() = 0;

        const std::string& Name();
        gl::FrameBufferPtr& FrameBuffer();
    private:
        std::string         m_Name;
        gl::FrameBufferPtr  m_pFrameBuffer;
    };
    typedef std::shared_ptr<IRenderingEffect>  IRenderingEffectPtr;

    typedef std::function<vd::gl::FrameBufferPtr()>         FrameBufferResolver;
    typedef std::vector<FrameBufferResolver>                FrameBufferResolvers;
    typedef std::function<vd::gl::FrameBufferPtrVec()>      FrameBuffersResolver;
    typedef std::function<void(vd::gl::FrameBufferPtr&)>    FrameBufferConfigurator;

    class ConcreteEffect : public IRenderingEffect {
    public:
        explicit ConcreteEffect(std::string name,
                                float scale,
                                FrameBuffersResolver inputGetter,
                                FrameBufferConfigurator frameBufferConfigurator,
                                Predicate precondition = vd::g_kEmptyPredicate,
                                Consumer beforeExecution = vd::g_kEmptyConsumer,
                                Consumer afterExecution = vd::g_kEmptyConsumer);

        virtual void Init(vd::Dimension dimension);

        bool Precondition() override;
        void Prepare() override;
        void Finish() override;

        vd::gl::FrameBufferPtr& InputFrameBuffer(size_t index = 0);
    private:
        float                       m_Scale;
        FrameBuffersResolver        m_fnResolver;
        FrameBufferConfigurator     m_fnConfigurator;
        Predicate                   m_fnPrecondition;
        Consumer                    m_fnBeforeExec;
        Consumer                    m_fnAfterExec;
        vd::gl::FrameBufferPtrVec   m_pInputFrameBuffers;
    };
    typedef std::shared_ptr<ConcreteEffect>   ConcreteEffectPtr;
}


#endif //VDGE_RENDERINGEFFECT_HPP

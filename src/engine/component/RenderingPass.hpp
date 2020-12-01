//
// Created by Vali on 11/12/2020.
//

#ifndef VD_GAME_ENGINE_RENDERINGPASS_HPP
#define VD_GAME_ENGINE_RENDERINGPASS_HPP

#include <engine/misc/Types.hpp>

#include <engine/api/gl/FrameBuffer.hpp>

#include <string>

namespace vd::component {
    class RenderingPass {
    public:
        typedef uint64_t priority_t;

        RenderingPass(std::string  name,
                      const priority_t& priority,
                      gl::FrameBufferPtr  frameBufferPtr,
                      vd::Predicate precondition = g_kEmptyPredicate,
                      vd::Consumer beforeExecution = g_kEmptyConsumer,
                      vd::Consumer afterExecution = g_kEmptyConsumer);
        ~RenderingPass();

        [[nodiscard]] bool Precondition() const;

        void Prepare();
        void Finish();

        [[nodiscard]] const std::string& Name() const;
        [[nodiscard]] const priority_t& Priority() const;

        [[nodiscard]] const gl::FrameBufferPtr& FrameBuffer() const;
    private:
        std::string m_Name;
        priority_t m_Priority;

        vd::Predicate m_Precondition;
        vd::Consumer m_BeforeExecution;
        vd::Consumer m_AfterExecution;

        gl::FrameBufferPtr m_FrameBufferPtr;
    };
}


#endif //VD_GAME_ENGINE_RENDERINGPASS_HPP

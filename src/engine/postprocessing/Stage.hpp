//
// Created by Vali on 2/19/2021.
//

#ifndef VDGE_STAGE_HPP
#define VDGE_STAGE_HPP

#include <memory>

#include <engine/api/gl/FrameBuffer.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/window/Window.hpp>

namespace vd::postprocessing {

    class Stage : public vd::injector::Injectable {
    public:
        explicit Stage(std::string name);

        virtual void Setup(vd::Dimension dimension) = 0;

        virtual bool Precondition() = 0;
        virtual void Prepare() = 0;
        virtual void Finish() = 0;

        void Init();
        void CleanUp();

        const std::string& Name();
        gl::FrameBufferPtr& FrameBuffer();
    private:
        std::string         m_Name;
        gl::FrameBufferPtr  m_pFrameBuffer;

        window::WindowPtr   m_pWindow;
    };
    typedef std::shared_ptr<Stage>  StagePtr;

}


#endif //VDGE_STAGE_HPP

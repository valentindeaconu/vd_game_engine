//
// Created by Vali on 9/15/2020.
//

#ifndef VD_GAME_ENGINE_FRAMEBUFFER_HPP
#define VD_GAME_ENGINE_FRAMEBUFFER_HPP

#include <memory>

#include "GL.hpp"
#include "Texture.hpp"

namespace vd::gl {
    enum DepthAttachment {
        eNone = 0,
        eDepthTexture,
        eDepthBuffer
    };

    class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();

        void Bind() const;
        static void Unbind();

        void Allocate(int width,
                      int height,
                      bool withColorTexture = true,
                      const DepthAttachment& depthAttachment = DepthAttachment::eNone);
        void CleanUp();

        [[nodiscard]] GLuint GetId() const;

        [[nodiscard]] const Texture2DPtr& GetColorTexture() const;

        [[nodiscard]] const Texture2DPtr& GetDepthTexture() const;

        [[nodiscard]] GLuint GetDepthBufferId() const;
    private:
        GLuint m_FboId;

        Texture2DPtr m_ColorTexture;

        size_t m_Width;
        size_t m_Height;

        DepthAttachment m_DepthAttachment;
        Texture2DPtr m_DepthTexture;
        GLuint m_DepthBufferId;
    };
    typedef std::shared_ptr<FrameBuffer>    FrameBufferPtr;
}


#endif //VD_GAME_ENGINE_FRAMEBUFFER_HPP

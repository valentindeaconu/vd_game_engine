//
// Created by Vali on 9/15/2020.
//

#include "FrameBuffer.hpp"

namespace vd::gl {

    FrameBuffer::FrameBuffer()
        : m_FboId(0)
        , m_ColorTexture(nullptr)
        , m_DepthTexture(nullptr)
        , m_DepthAttachment()
        , m_DepthBufferId(0)
        , m_Width(0)
        , m_Height(0)
    {
    }

    FrameBuffer::~FrameBuffer() = default;

    void FrameBuffer::Bind() const {
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);
        glViewport(0, 0, m_Width, m_Height);
    }

    void FrameBuffer::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::Allocate(int width, int height, bool withColorTexture, const DepthAttachment& depthAttachment) {
        this->m_Width = width;
        this->m_Height = height;
        this->m_DepthAttachment = depthAttachment;

        glGenFramebuffers(1, &m_FboId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);

        if (withColorTexture) {
            glDrawBuffer(GL_COLOR_ATTACHMENT0);
            this->m_ColorTexture = TextureService::Get(width, height, Attachment::eColor);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->m_ColorTexture->Id(), 0);
        } else {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }

        switch (depthAttachment) {
            case eNone:
                break;
            case eDepthTexture:
                this->m_DepthTexture = TextureService::Get(width, height, Attachment::eDepth);
                glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->m_DepthTexture->Id(), 0);
                break;
            case eDepthBuffer:
                glGenRenderbuffers(1, &this->m_DepthBufferId);
                glBindRenderbuffer(GL_RENDERBUFFER, this->m_DepthBufferId);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->m_DepthBufferId);
                break;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::CleanUp() {
        glDeleteFramebuffers(1, &m_FboId);

        if (this->m_DepthAttachment == DepthAttachment::eDepthBuffer) {
            glDeleteRenderbuffers(1, &this->m_DepthBufferId);
        }
    }

    GLuint FrameBuffer::GetId() const {
        return m_FboId;
    }

    const Texture2DPtr& FrameBuffer::GetColorTexture() const {
        return m_ColorTexture;
    }

    const Texture2DPtr& FrameBuffer::GetDepthTexture() const {
        return m_DepthTexture;
    }

    GLuint FrameBuffer::GetDepthBufferId() const {
        return m_DepthBufferId;
    }

}
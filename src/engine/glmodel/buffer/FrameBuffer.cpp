//
// Created by Vali on 9/15/2020.
//

#include "FrameBuffer.hpp"

namespace vd::buffer {

    FrameBuffer::FrameBuffer()
        : fboId(0)
        , colorTexture(nullptr)
        , depthTexture(nullptr)
        , depthAttachment()
        , depthBufferId(0)
        , width(0)
        , height(0)
    {
    }

    FrameBuffer::~FrameBuffer()
    {
    }

    void FrameBuffer::bind() const {
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, fboId);
        glViewport(0, 0, width, height);
    }

    void FrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::allocate(int width, int height, bool withColorTexture, const DepthAttachment& depthAttachment) {
        this->width = width;
        this->height = height;
        this->depthAttachment = depthAttachment;

        glGenFramebuffers(1, &fboId);
        glBindFramebuffer(GL_FRAMEBUFFER, fboId);

        if (withColorTexture) {
            glDrawBuffer(GL_COLOR_ATTACHMENT0);
            this->colorTexture = model::TextureService::get(width, height, model::Attachment::eColor);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->colorTexture->getId(), 0);
        } else {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }

        switch (depthAttachment) {
            case eNone:
                break;
            case eDepthTexture:
                this->depthTexture = model::TextureService::get(width, height, model::Attachment::eDepth);
                glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthTexture->getId(), 0);
                break;
            case eDepthBuffer:
                glGenRenderbuffers(1, &this->depthBufferId);
                glBindRenderbuffer(GL_RENDERBUFFER, this->depthBufferId);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depthBufferId);
                break;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::cleanUp() {
        glDeleteFramebuffers(1, &fboId);

        if (this->depthAttachment == DepthAttachment::eDepthBuffer) {
            glDeleteRenderbuffers(1, &this->depthBufferId);
        }
    }

    GLuint FrameBuffer::getId() const {
        return fboId;
    }

    const model::Texture2DPtr &FrameBuffer::getColorTexture() const {
        return colorTexture;
    }

    const model::Texture2DPtr &FrameBuffer::getDepthTexture() const {
        return depthTexture;
    }

    GLuint FrameBuffer::getDepthBufferId() const {
        return depthBufferId;
    }

}
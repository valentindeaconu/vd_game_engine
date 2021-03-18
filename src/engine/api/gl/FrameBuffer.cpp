//
// Created by Vali on 9/15/2020.
//

#include "FrameBuffer.hpp"

namespace vd::gl {

    FrameBuffer::FrameBuffer(size_t width, size_t height, const Type& type)
        : m_Type(type)
        , m_Id(0)
        , m_Dimension(width, height)
        , m_DepthBufferId(0)
        , m_Bound(false)
        , m_HasDepthBuffer(false)
        , m_HasDepthTexture(false)
        , m_ColorAttachments(0)
    {
    }

    void FrameBuffer::OnCreate() {
        glGenFramebuffers(1, &m_Id);

        glBindFramebuffer(m_Type, m_Id);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_DEPTH_TEST);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(m_Type, 0);
    }

    void FrameBuffer::OnCleanUp() {
        glBindFramebuffer(m_Type, m_Id);

        if (m_HasDepthBuffer) {
            glDeleteRenderbuffers(1, &this->m_DepthBufferId);
        }

        glBindFramebuffer(m_Type, 0);

        m_Textures.clear();

        glDeleteFramebuffers(1, &m_Id);
    }

    void FrameBuffer::Bind() {
        if (!m_Bound) {
            m_Bound = true;

            glBindFramebuffer(m_Type, m_Id);
            glViewport(0, 0, m_Dimension.width, m_Dimension.height);

            if (m_HasDepthTexture || m_HasDepthBuffer) {
                glEnable(GL_DEPTH_TEST);
            }
        }
    }

    void FrameBuffer::Unbind() {
        if (m_Bound) {
            m_Bound = false;

            glBindFramebuffer(m_Type, 0);
        }
    }

    void FrameBuffer::Clear() const {
        if (m_Id == 0) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            return;
        }

        if (m_ColorAttachments > 0) {
            glClear(GL_COLOR_BUFFER_BIT);
        }

        if (m_HasDepthTexture || m_HasDepthBuffer) {
            glClear(GL_DEPTH_BUFFER_BIT);
        }
    }

    bool FrameBuffer::IsBound() const {
        return m_Bound;
    }

    void FrameBuffer::PushAttachment(const FrameBuffer::Attachment& attachment, const TextureConfigurator& configurator) {
        PassIfCreated();

        if (m_Id == 0) {
            throw RuntimeError("cannot push attachment into the default framebuffer (id 0)");
        }

        bool revert = false;
        if (!m_Bound) {
            glBindFramebuffer(m_Type, m_Id);
            revert = true;
        }

        switch (attachment) {
            case eColorTexture: {
                glDrawBuffer(GL_COLOR_ATTACHMENT0);

                Texture2DPtr pTex = service::TextureService::UncachedCreate(m_Dimension,
                                                                            gl::TextureFormat::eRGB,
                                                                            gl::TextureFormat::eRGB,
                                                                            gl::DataType::eUnsignedByte);

                configurator(pTex);

                GLuint index = GL_COLOR_ATTACHMENT0 + m_ColorAttachments;
                glFramebufferTexture(m_Type, index, pTex->Id(), 0);

                m_Textures[index] = pTex;
                m_ColorAttachments++;
                break;
            }
            case eDepthTexture: {
                m_HasDepthTexture = true;

                Texture2DPtr pTex = service::TextureService::UncachedCreate(m_Dimension,
                                                                            gl::TextureFormat::eDepthComponent,
                                                                            gl::TextureFormat::eDepthComponent,
                                                                            gl::DataType::eFloat);

                configurator(pTex);

                GLuint index = kDepthAttachment;
                glFramebufferTexture(m_Type, index, pTex->Id(), 0);

                m_Textures[index] = pTex;
                break;
            }
            case eDepthBuffer: {
                glGenRenderbuffers(1, &this->m_DepthBufferId);
                glBindRenderbuffer(GL_RENDERBUFFER, this->m_DepthBufferId);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Dimension.width, m_Dimension.height);
                glFramebufferRenderbuffer(m_Type, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->m_DepthBufferId);
                m_HasDepthBuffer = true;
                break;
            }
        }

        if (revert) {
            glBindFramebuffer(m_Type, m_Id);
        }
    }

    bool FrameBuffer::Commit() const {
        PassIfCreated();

        StatusType status = Status();

        if (status != FrameBuffer::eComplete) {
            throw RuntimeError("FrameBuffer with id " + std::to_string(m_Id) + " is incomplete: " + StatusToString(status));
        }

        return true;
    }

    FrameBuffer::StatusType FrameBuffer::Status() const {
        if (m_Id == 0) {
            return eComplete;
        }

        switch (glCheckFramebufferStatus(m_Type)) {
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: return eIncompleteAttachment;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return eIncompleteMissingAttachment;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: return eIncompleteDrawBuffer;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: return eIncompleteReadBuffer;
            case GL_FRAMEBUFFER_UNSUPPORTED: return eUnsupported;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: return eIncompleteMultiSample;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: return eIncompleteLayerTargets;
            case GL_FRAMEBUFFER_COMPLETE: return eComplete;
            case GL_FRAMEBUFFER_UNDEFINED:
            default:
                return eUndefined;
        }
    }

     std::string FrameBuffer::StatusAsString() const {
         return StatusToString(Status());
    }

    std::string FrameBuffer::StatusToString(FrameBuffer::StatusType statusType) {
        switch (statusType) {
            case eIncompleteAttachment: return "Incomplete Attachment";
            case eIncompleteMissingAttachment: return "Incomplete Missing Attachment";
            case eIncompleteDrawBuffer: return "Incomplete Draw Buffer";
            case eIncompleteReadBuffer: return "Incomplete Read Buffer";
            case eUnsupported: return "Unsupported";
            case eIncompleteMultiSample: return "Incomplete MultiSample";
            case eIncompleteLayerTargets: return "Incomplete Layer Targets";
            case eComplete: return "Complete";
            case eUndefined:
            default:
                return "Undefined";
        }
    }

    GLuint FrameBuffer::Id() const {
        return m_Id;
    }

    const vd::Dimension& FrameBuffer::Dimension() const {
        return m_Dimension;
    }

    Texture2DPtr& FrameBuffer::ColorTexture(GLuint index) {
        PassIfCreated();

        const GLuint key = GL_COLOR_ATTACHMENT0 + index;
        if (m_ColorAttachments == 0 || !m_Textures.contains(key)) {
            throw RuntimeError("color attachment with index " + std::to_string(index) + " is not allocated");
        }

        return m_Textures[key];
    }

    Texture2DPtr& FrameBuffer::DepthTexture() {
        PassIfCreated();

        if (!m_HasDepthTexture || !m_Textures.contains(kDepthAttachment)) {
            throw RuntimeError("depth texture attachment is not allocated");
        }

        return m_Textures[kDepthAttachment];
    }

    GLuint& FrameBuffer::DepthBuffer() {
        PassIfCreated();

        if (!m_HasDepthBuffer) {
            throw RuntimeError("depth buffer attachment is not allocated");
        }

        return m_DepthBufferId;
    }

    void FrameBuffer::Resize(size_t width, size_t height) {
        if (m_Id == 0) {
            m_Dimension = vd::Dimension(width, height);
            return;
        }

        // TODO: This needs rework, FN, attachments are resized, but their configuration is not kept
        bool revert = false;
        if (!m_Bound) {
            glBindFramebuffer(m_Type, m_Id);
            revert = true;
        }

        m_Dimension = vd::Dimension(width, height);

        if (!m_Textures.empty()) {
            auto backup = m_Textures;
            m_Textures.clear();
            m_Textures.reserve(backup.size());
            m_ColorAttachments = 0;
            for (auto& p : backup) {
                if (p.first == kDepthAttachment) {
                    PushAttachment(eDepthTexture);
                } else {
                    PushAttachment(eColorTexture);
                }
            }
        }

        if (m_HasDepthBuffer) {
            m_HasDepthBuffer = false;
            glDeleteRenderbuffers(1, &this->m_DepthBufferId);
            PushAttachment(eDepthBuffer);
        }

        if (revert) {
            glBindFramebuffer(m_Type, m_Id);
        }
    }

}
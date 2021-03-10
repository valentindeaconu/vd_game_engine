//
// Created by Vali on 9/15/2020.
//

#ifndef VD_GAME_ENGINE_FRAMEBUFFER_HPP
#define VD_GAME_ENGINE_FRAMEBUFFER_HPP

#include <memory>
#include <unordered_map>

#include "GL.hpp"
#include "GLComponent.hpp"
#include "Texture.hpp"

#include <engine/service/TextureService.hpp>

namespace vd::gl {

    class FrameBuffer : public GLComponent {
    public:
        typedef std::function<void(Texture2DPtr& t)>    TextureConfigurator;
        static inline const TextureConfigurator g_kDefaultConfigurator = [](Texture2DPtr&) { };

        enum Type {
            eRead = GL_READ_FRAMEBUFFER,
            eDraw = GL_DRAW_FRAMEBUFFER,
            eReadWrite = GL_FRAMEBUFFER,
            eDefault = 0
        };

        enum Attachment {
            eColorTexture = 0,
            eDepthTexture,
            eDepthBuffer
        };

        enum StatusType {
            eUndefined = GL_FRAMEBUFFER_UNDEFINED,
            eIncompleteAttachment = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
            eIncompleteMissingAttachment = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
            eIncompleteDrawBuffer = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,
            eIncompleteReadBuffer = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,
            eUnsupported = GL_FRAMEBUFFER_UNSUPPORTED,
            eIncompleteMultiSample = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,
            eIncompleteLayerTargets = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS,
            eComplete = GL_FRAMEBUFFER_COMPLETE,
        };

        FrameBuffer(size_t width, size_t height, const Type& type = eReadWrite);

        void Create() override;
        void CleanUp() override;

        void Bind();
        void Unbind();
        void Clear() const;

        [[nodiscard]] bool IsBound() const;

        void PushAttachment(const Attachment& attachment, const TextureConfigurator& configurator = g_kDefaultConfigurator);
        void Resize(size_t width, size_t height);

        /// Method used to confirm the validity of the FrameBuffer object
        [[nodiscard]] bool Commit() const;

        [[nodiscard]] StatusType Status() const;
        [[nodiscard]] std::string StatusAsString() const;
        [[nodiscard]] static std::string StatusToString(StatusType statusType);

        [[nodiscard]] GLuint Id() const;
        [[nodiscard]] const vd::Dimension& Dimension() const;

        Texture2DPtr& ColorTexture(GLuint index = 0);
        Texture2DPtr& DepthTexture();

        GLuint& DepthBuffer();
    private:
        const GLuint kDepthAttachment = GL_DEPTH_ATTACHMENT;

        Type            m_Type;
        GLuint          m_Id;
        GLuint          m_DepthBufferId;
        vd::Dimension   m_Dimension;

        bool        m_HasDepthBuffer;
        bool        m_HasDepthTexture;
        bool        m_Bound;
        uint32_t    m_ColorAttachments;

        std::unordered_map<GLuint, Texture2DPtr>    m_Textures;
    };
    typedef std::shared_ptr<FrameBuffer>        FrameBufferPtr;
    typedef std::vector<FrameBufferPtr>         FrameBufferPtrVec;

}


#endif //VD_GAME_ENGINE_FRAMEBUFFER_HPP

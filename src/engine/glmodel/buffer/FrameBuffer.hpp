//
// Created by Vali on 9/15/2020.
//

#ifndef VD_GAME_ENGINE_FRAMEBUFFER_HPP
#define VD_GAME_ENGINE_FRAMEBUFFER_HPP

#include <engine/foundation/GL.hpp>

#include <engine/model/Texture.hpp>

#include <memory>

namespace vd::buffer {
    enum DepthAttachment {
        eDepthTexture = 0,
        eDepthBuffer
    };

    class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();

        void bind() const;
        static void unbind();

        void allocate(int width, int height, const DepthAttachment& depthAttachment);
        void cleanUp();

        [[nodiscard]] GLuint getId() const;

        [[nodiscard]] const model::Texture2DPtr& getColorTexture() const;

        [[nodiscard]] const model::Texture2DPtr& getDepthTexture() const;
        [[nodiscard]] GLuint getDepthBufferId() const;
    private:
        GLuint fboId;

        model::Texture2DPtr colorTexture;

        size_t width;
        size_t height;

        DepthAttachment depthAttachment;
        model::Texture2DPtr depthTexture;
        GLuint depthBufferId;
    };
    typedef std::shared_ptr<FrameBuffer>    FrameBufferPtr;
}


#endif //VD_GAME_ENGINE_FRAMEBUFFER_HPP

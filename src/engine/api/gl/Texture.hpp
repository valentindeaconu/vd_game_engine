//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_GL_TEXTURE_HPP
#define VD_GAME_ENGINE_GL_TEXTURE_HPP

#include <engine/loader/ImageLoader.hpp>

#include <engine/misc/Types.hpp>

#include <string>
#include <algorithm>

#include "GL.hpp"

namespace vd::gl {
    enum Attachment {
        eDepth = 0,
        eColor
    };

    void ActiveTexture(GLuint index);

    template<GLuint T>
    class Texture {
    public:
        Texture();
        Texture(const Texture& other);
        explicit Texture(const vd::model::ImagePtr<uint8_t>& imagePtr);
        explicit Texture(const vd::model::ImagePtr<float>& imagePtr);
        explicit Texture(const vd::model::ImagePtr<float, vd::model::ImageFormat::eR>& imagePtr);
        Texture(size_t width, size_t height);
        ~Texture();

        Texture& operator=(const Texture& other);

        void Generate();
        void Bind() const;
        void Unbind() const;

        void NoFilter();
        void BilinearFilter();
        void TrilinearFilter();
        void TrilinearFilterWithAnisotropy();

        void WrapRepeat();
        void WrapMirroredRepeat();
        void WrapClampToEdge();
        void WrapClampToBorder();

        [[nodiscard]] GLuint Id() const;

        [[nodiscard]] size_t Width() const;
        [[nodiscard]] size_t Height() const;

        friend class TextureService;

    private:
        GLuint m_Id;

        vd::Dimension m_Dimension;
    };

    typedef	Texture<GL_TEXTURE_2D>		Texture2D;
    typedef std::shared_ptr<Texture2D>	Texture2DPtr;

    class ShadowTexture : public Texture2D {
    public:
        ShadowTexture(size_t width, size_t height);
        ShadowTexture(const vd::model::ImagePtr<uint8_t>& imagePtr) = delete;
    };
    typedef std::shared_ptr<ShadowTexture>	ShadowTexturePtr;

    class UTexture2D : public Texture2D {
    public:
        UTexture2D(size_t width, size_t height, const std::vector<uint16_t>& imagePtr);
        UTexture2D(const vd::model::ImagePtr<uint8_t>& imagePtr) = delete;
    };
    typedef std::shared_ptr<UTexture2D>	UTexture2DPtr;

    class TextureService {
    public:
        ~TextureService();

        static Texture2DPtr Get(const std::string& path);
        static Texture2DPtr Get(const vd::model::ImagePtr<uint8_t>& imagePtr);
        static Texture2DPtr Get(size_t width, size_t height, Attachment attachment);
        static UTexture2DPtr Get(size_t width, size_t height, const std::vector<uint16_t>& data);

        static void Remove(Texture2DPtr& texture);

    private:
        TextureService();

        static TextureService& getInstance();

        std::unordered_map<std::string, Texture2DPtr> m_Cache;
    };
}

#endif // !__TEXTURE_HPP_

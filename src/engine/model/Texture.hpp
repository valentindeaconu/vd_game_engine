#ifndef __TEXTURE_HPP_
#define __TEXTURE_HPP_

#include <engine/foundation/GL.hpp>
#include <engine/foundation/img/imgloader/IMGLoader.hpp>

#include <string>
#include <algorithm>

namespace vd::model
{
    enum Attachment {
        eDepth = 0,
        eColor
    };

    void activeTexture(GLuint index);

    template<GLuint T>
    class Texture
    {
    public:
        Texture();
        Texture(const Texture& other);
        Texture(const vd::img::ImageBPtr& imagePtr);
        Texture(size_t width, size_t height);
        ~Texture();

        Texture& operator=(const Texture& other);

        void generate();
        void bind() const;
        void unbind() const;

        void noFilter();
        void bilinearFilter();
        void trilinearFilter();
        void trilinearFilterWithAnisotropy();

        void wrapRepeat();
        void wrapMirroredRepeat();
        void wrapClampToEdge();
        void wrapClampToBorder();

        GLuint getId() const;

        size_t getWidth() const;
        size_t getHeight() const;

        friend class TextureService;

    private:
        GLuint id;

        size_t width;
        size_t height;
    };

    typedef	Texture<GL_TEXTURE_2D>		Texture2D;
    typedef std::shared_ptr<Texture2D>	Texture2DPtr;

    class ShadowTexture : public Texture2D
    {
    public:
        ShadowTexture(size_t width, size_t height);
        ShadowTexture(const vd::img::ImageBPtr& imagePtr) = delete;
    };
    typedef std::shared_ptr<ShadowTexture>	ShadowTexturePtr;

    class UTexture2D : public Texture2D
    {
    public:
        UTexture2D(size_t width, size_t height, const std::vector<uint16_t>& imagePtr);
        UTexture2D(const vd::img::ImageBPtr& imagePtr) = delete;
    };
    typedef std::shared_ptr<UTexture2D>	UTexture2DPtr;

    class TextureService
    {
    public:
        ~TextureService();

        static TextureService& getInstance();

        static Texture2DPtr get(const std::string& path);
        static Texture2DPtr get(const img::ImageBPtr& imagePtr);
        static UTexture2DPtr get(size_t width, size_t height, const std::vector<uint16_t>& data);
        static Texture2DPtr get(size_t width, size_t height, Attachment attachment);

        static void remove(Texture2DPtr& texture);

    private:
        TextureService();

        img::IMGLoaderPtr imgLoaderPtr;

        std::unordered_map<std::string, Texture2DPtr> cache;
    };
}

#endif // !__TEXTURE_HPP_

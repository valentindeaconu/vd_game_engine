#include "Texture.hpp"

namespace vd::model
{
    void activeTexture(GLuint index)
    {
        glActiveTexture(GL_TEXTURE0 + index);
    }

    template class Texture<GL_TEXTURE_2D>;

    template<GLuint type>
    Texture<type>::Texture()
        : width(width)
        , height(height)
        , id(0)
    {
    }

    template<GLuint type>
    Texture<type>::Texture(const Texture<type>& other)
        : width(other.width)
        , height(other.height)
    {
        this->id = other.id;
    }

    template<GLuint type>
    Texture<type>::Texture(const vd::imgloader::ImageBPtr& imagePtr)
        : width(imagePtr->width)
        , height(imagePtr->height)
    {
        generate();
        bind();

        // GL_SRGB, GL_RGBA
        glTexImage2D(type, 0, GL_SRGB_ALPHA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(imagePtr->data[0]));

        wrapRepeat();
        unbind();
    }

    template<GLuint type>
    Texture<type>::Texture(size_t width, size_t height)
        : width(width)
        , height(height)
    {
    }

    template<GLuint type>
    Texture<type>& Texture<type>::operator=(const Texture<type>& other)
    {
        this->id = other.id;
        this->width = other.width;
        this->height = other.height;

        return *this;
    }

    template<GLuint type>
    Texture<type>::~Texture()
    {
        glDeleteTextures(1, &id);
    }

    template<GLuint type>
    void Texture<type>::generate()
    {
        glGenTextures(1, &id);
    }

    template<GLuint type>
    void Texture<type>::bind() const
    {
        glBindTexture(type, id);
    }

    template<GLuint type>
    void Texture<type>::unbind() const
    {
        glBindTexture(type, 0);
    }

    template<GLuint type>
    void Texture<type>::noFilter()
    {
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    template<GLuint type>
    void Texture<type>::bilinearFilter()
    {
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    template<GLuint type>
    void Texture<type>::trilinearFilter()
    {
        glGenerateMipmap(type);
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    template<GLuint type>
    void Texture<type>::wrapRepeat()
    {
        glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_REPEAT);
    }

    template<GLuint type>
    void Texture<type>::wrapMirroredRepeat()
    {
        glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        //glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
    }

    template<GLuint type>
    void Texture<type>::wrapClampToEdge()
    {
        glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    template<GLuint type>
    void Texture<type>::wrapClampToBorder()
    {
        glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        //glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    }

    template<GLuint type>
    uint32_t Texture<type>::getId() const
    {
        return id;
    }

    template<GLuint type>
    size_t Texture<type>::getWidth() const
    {
        return width;
    }

    template<GLuint type>
    size_t Texture<type>::getHeight() const
    {
        return height;
    }

    ShadowTexture::ShadowTexture(size_t width, size_t height)
        : Texture<GL_TEXTURE_2D>(width, height)
    {
        generate();
        bind();

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_DEPTH_COMPONENT,
                     (GLsizei)width,
                     (GLsizei)height,
                     0,
                     GL_DEPTH_COMPONENT,
                     GL_FLOAT,
                     NULL);

        noFilter();
        wrapClampToBorder();

        float border[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

        unbind();
    }

    UTexture2D::UTexture2D(size_t width, size_t height, const std::vector<uint16_t>& data)
        : Texture2D(width, height)
    {
        generate();
        bind();

        // GL_SRGB, GL_RGBA
        glTexImage2D(GL_TEXTURE_2D,
            0,
            GL_R16UI,
            (GLsizei)width,
            (GLsizei)height,
            0,
            GL_RED_INTEGER,
            GL_UNSIGNED_SHORT,
            &data[0]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

        noFilter();
        wrapRepeat();
        unbind();
    }

    TextureService::TextureService()
    {
        imgLoaderPtr = std::make_shared<imgloader::IMGLoader>();
    }

    TextureService& TextureService::getInstance()
    {
        static TextureService instance;

        return instance;
    }

    TextureService::~TextureService()
    {
        imgLoaderPtr = nullptr;
        cache.clear();
    }

    Texture2DPtr TextureService::get(const std::string& path)
    {
        TextureService& instance = getInstance();

        if (instance.cache.find(path) != instance.cache.end())
        {
            return instance.cache[path];
        }

        vd::imgloader::ImageBPtr imagePtr = instance.imgLoaderPtr->loadByteImage(path);

        Texture2DPtr texture = std::make_shared<Texture2D>(imagePtr);

        instance.cache[path] = texture;

        return texture;
    }

    Texture2DPtr TextureService::get(const imgloader::ImageBPtr& imagePtr)
    {
        Texture2DPtr texture = std::make_shared<Texture2D>(imagePtr);
        return texture;
    }

    UTexture2DPtr TextureService::get(size_t width, size_t height, const std::vector<uint16_t>& data)
    {
        UTexture2DPtr texture = std::make_shared<UTexture2D>(width, height, data);
        return texture;
    }

    Texture2DPtr TextureService::get(size_t width, size_t height, Attachment attachment)
    {
        Texture2DPtr texture = std::make_shared<Texture2D>(width, height);

        texture->generate();
        texture->bind();

        switch (attachment) {
            case eDepth:
                glTexImage2D(GL_TEXTURE_2D,
                             0,
                             GL_DEPTH_COMPONENT32,
                             width,
                             height,
                             0,
                             GL_DEPTH_COMPONENT,
                             GL_FLOAT,
                             nullptr);
                break;
            case eColor:
                glTexImage2D(GL_TEXTURE_2D,
                             0,
                             GL_RGB,
                             width,
                             height,
                             0,
                             GL_RGB,
                             GL_UNSIGNED_BYTE,
                             nullptr);
                break;
        }


        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

        texture->bilinearFilter();

        return texture;
    }

    void TextureService::remove(Texture2DPtr& texture)
    {
        TextureService& instance = getInstance();

        auto it = instance.cache.begin();
        for (; it != instance.cache.end(); ++it)
        {
            if (it->second->getId() == texture->getId())
            {
                break;
            }
        }

        if (it != instance.cache.end())
        {
            instance.cache.erase(it);
        }

        texture = nullptr;
    }
}
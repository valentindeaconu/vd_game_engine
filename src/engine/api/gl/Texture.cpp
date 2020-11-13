#include "Texture.hpp"

namespace vd::gl {

    void ActiveTexture(GLuint index) {
        glActiveTexture(GL_TEXTURE0 + index);
    }

    template class Texture<GL_TEXTURE_2D>;

    template<GLuint type>
    Texture<type>::Texture()
        : m_Id(0)
        , m_Dimension(0, 0)
    {
    }

    template<GLuint type>
    Texture<type>::Texture(const Texture<type>& other)
        : m_Dimension(other.m_Dimension)
        , m_Id(other.m_Id)
    {
    }

    template<GLuint type>
    Texture<type>::Texture(const vd::model::ImagePtr<uint8_t>& imagePtr)
        : m_Dimension(imagePtr->Width(), imagePtr->Height())
        , m_Id(0)
    {
        Generate();
        Bind();

        // GL_SRGB, GL_RGBA
        glTexImage2D(type,
                     0,
                     GL_SRGB_ALPHA,
                     GLsizei(m_Dimension.width),
                     GLsizei(m_Dimension.height),
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     &(imagePtr->Data()[0]));

        NoFilter();
        WrapRepeat();
        Unbind();
    }

    template<GLuint type>
    Texture<type>::Texture(const vd::model::ImagePtr<float>& imagePtr)
        : m_Dimension(imagePtr->Width(), imagePtr->Height())
        , m_Id(0)
    {
        Generate();
        Bind();

        const size_t sz = imagePtr->Width() * imagePtr->Height();
        std::vector<float> data;
        data.reserve(sz);
        for (int i = 0; i < sz * 4; i+=4) {
            data.emplace_back(imagePtr->Data()[i]);
        }

        glTexImage2D(type,
                     0,
                     GL_R16F,
                     GLsizei(m_Dimension.width),
                     GLsizei(m_Dimension.height),
                     0,
                     GL_RED,
                     GL_FLOAT,
                     &data[0]);

        BilinearFilter();
        WrapRepeat();
        Unbind();
    }

    template<GLuint T>
    Texture<T>::Texture(const model::ImagePtr<float, model::ImageFormat::eR>& imagePtr)
        : m_Dimension(imagePtr->Dimension())
        , m_Id(0)
    {
        Generate();
        Bind();

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_R32F,
                     GLsizei(m_Dimension.width),
                     GLsizei(m_Dimension.height),
                     0,
                     GL_RED,
                     GL_FLOAT,
                     &imagePtr->Data()[0]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

        BilinearFilter();
        WrapRepeat();
        Unbind();
    }

    template<GLuint type>
    Texture<type>::Texture(size_t width, size_t height)
        : m_Dimension(width, height)
        , m_Id(0)
    {
    }

    template<GLuint type>
    Texture<type>& Texture<type>::operator=(const Texture<type>& other) {
        this->m_Id = other.m_Id;
        this->m_Dimension = other.m_Dimension;

        return *this;
    }

    template<GLuint type>
    Texture<type>::~Texture() {
        glDeleteTextures(1, &m_Id);
    }

    template<GLuint type>
    void Texture<type>::Generate() {
        glGenTextures(1, &m_Id);
    }

    template<GLuint type>
    void Texture<type>::Bind() const {
        glBindTexture(type, m_Id);
    }

    template<GLuint type>
    void Texture<type>::Unbind() const {
        glBindTexture(type, 0);
    }

    template<GLuint type>
    void Texture<type>::NoFilter() {
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    template<GLuint type>
    void Texture<type>::BilinearFilter() {
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    template<GLuint type>
    void Texture<type>::TrilinearFilter() {
        glGenerateMipmap(type);
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    template<GLuint type>
    void Texture<type>::TrilinearFilterWithAnisotropy() {
        const GLfloat kMaxAnisotropy = 8.0f;
        GLfloat value;

        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &value);
        value = std::min(kMaxAnisotropy, value);

        glTexParameterf(type, GL_TEXTURE_LOD_BIAS, 0);
        glGenerateMipmap(type);
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, value);
    }


    template<GLuint type>
    void Texture<type>::WrapRepeat() {
        glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_REPEAT);
    }

    template<GLuint type>
    void Texture<type>::WrapMirroredRepeat() {
        glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        //glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
    }

    template<GLuint type>
    void Texture<type>::WrapClampToEdge() {
        glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    template<GLuint type>
    void Texture<type>::WrapClampToBorder() {
        glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        //glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    }

    template<GLuint type>
    uint32_t Texture<type>::Id() const {
        return m_Id;
    }

    template<GLuint type>
    size_t Texture<type>::Width() const {
        return m_Dimension.width;
    }

    template<GLuint type>
    size_t Texture<type>::Height() const {
        return m_Dimension.height;
    }

    ShadowTexture::ShadowTexture(size_t width, size_t height)
        : Texture<GL_TEXTURE_2D>(width, height)
    {
        Generate();
        Bind();

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_DEPTH_COMPONENT,
                     GLsizei(width),
                     GLsizei(height),
                     0,
                     GL_DEPTH_COMPONENT,
                     GL_FLOAT,
                     nullptr);

        NoFilter();
        WrapClampToBorder();

        float border[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

        Unbind();
    }

    UTexture2D::UTexture2D(size_t width, size_t height, const std::vector<uint16_t>& data)
        : Texture2D(width, height)
    {
        Generate();
        Bind();

        // GL_SRGB, GL_RGBA
        glTexImage2D(GL_TEXTURE_2D,
            0,
            GL_R16UI,
            GLsizei(width),
            GLsizei(height),
            0,
            GL_RED_INTEGER,
            GL_UNSIGNED_SHORT,
            &data[0]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

        NoFilter();
        WrapRepeat();
        Unbind();
    }

    TextureService::TextureService() = default;

    TextureService& TextureService::getInstance() {
        static TextureService instance;
        return instance;
    }

    TextureService::~TextureService() {
        m_Cache.clear();
    }

    Texture2DPtr TextureService::Get(const std::string& path) {
        TextureService& instance = getInstance();

        if (instance.m_Cache.find(path) != instance.m_Cache.end()) {
            return instance.m_Cache[path];
        }


        vd::model::ImagePtr<uint8_t> imagePtr = loader::ImageLoader::Load<uint8_t>(path);

        Texture2DPtr texture = std::make_shared<Texture2D>(imagePtr);

        instance.m_Cache[path] = texture;

        return texture;
    }

    Texture2DPtr TextureService::Get(const vd::model::ImagePtr<uint8_t>& imagePtr) {
        Texture2DPtr texture = std::make_shared<Texture2D>(imagePtr);
        return texture;
    }

    UTexture2DPtr TextureService::Get(size_t width, size_t height, const std::vector<uint16_t>& data) {
        UTexture2DPtr texture = std::make_shared<UTexture2D>(width, height, data);
        return texture;
    }

    Texture2DPtr TextureService::Get(size_t width, size_t height, Attachment attachment) {
        Texture2DPtr texture = std::make_shared<Texture2D>(width, height);

        texture->Generate();
        texture->Bind();

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

        texture->BilinearFilter();

        return texture;
    }

    void TextureService::Remove(Texture2DPtr& texture) {
        TextureService& instance = getInstance();

        auto it = std::find_if(instance.m_Cache.begin(), instance.m_Cache.end(), [&](const auto& pair) {
            return pair.second->Id() == texture->Id();
        });

        if (it != instance.m_Cache.end()) {
            instance.m_Cache.erase(it);
        }

        texture = nullptr;
    }
}
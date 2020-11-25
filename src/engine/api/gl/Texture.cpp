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
    void Texture<type>::Parameter(const TextureParameter& parameter, int value) {
        glTexParameteri(type, parameter, value);
    }

    template<GLuint type>
    void Texture<type>::Parameter(const TextureParameter &parameter, float value) {
        glTexParameterf(type, parameter, value);
    }

    template<GLuint type>
    void Texture<type>::Parameter(const TextureParameter &parameter, const int *value) {
        glTexParameteriv(type, parameter, value);
    }

    template<GLuint type>
    void Texture<type>::Parameter(const TextureParameter &parameter, const float *value) {
        glTexParameterfv(type, parameter, value);
    }

    template<GLuint type>
    void Texture<type>::NoFilter() {
        this->Parameter(TextureParameter::eTextureMinFilter, GL_NEAREST);
        this->Parameter(TextureParameter::eTextureMagFilter, GL_NEAREST);
        // glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    template<GLuint type>
    void Texture<type>::BilinearFilter() {
        this->Parameter(TextureParameter::eTextureMinFilter, GL_LINEAR);
        this->Parameter(TextureParameter::eTextureMagFilter, GL_LINEAR);
        // glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    template<GLuint type>
    void Texture<type>::TrilinearFilter() {
        glGenerateMipmap(type);
        this->Parameter(TextureParameter::eTextureMinFilter, GL_LINEAR_MIPMAP_LINEAR);
        this->Parameter(TextureParameter::eTextureMagFilter, GL_LINEAR);

        //glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    template<GLuint type>
    void Texture<type>::TrilinearFilterWithAnisotropy() {
        const GLfloat kMaxAnisotropy = 8.0f;
        GLfloat value;

        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &value);
        value = std::min(kMaxAnisotropy, value);

        this->Parameter(TextureParameter::eTextureLodBias, 0);
        // glTexParameterf(type, GL_TEXTURE_LOD_BIAS, 0);

        glGenerateMipmap(type);
        this->Parameter(TextureParameter::eTextureMinFilter, GL_LINEAR_MIPMAP_LINEAR);
        this->Parameter(TextureParameter::eTextureMaxAnisotropy, value);
        //glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, value);
    }


    template<GLuint type>
    void Texture<type>::WrapRepeat() {
        //glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
        //glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
        this->Parameter(TextureParameter::eTextureWrapS, GL_REPEAT);
        this->Parameter(TextureParameter::eTextureWrapT, GL_REPEAT);

        //glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_REPEAT);
    }

    template<GLuint type>
    void Texture<type>::WrapMirroredRepeat() {
        this->Parameter(TextureParameter::eTextureWrapS, GL_MIRRORED_REPEAT);
        this->Parameter(TextureParameter::eTextureWrapT, GL_MIRRORED_REPEAT);

        //glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        //glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        //glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
    }

    template<GLuint type>
    void Texture<type>::WrapClampToEdge() {
        this->Parameter(TextureParameter::eTextureWrapS, GL_CLAMP_TO_EDGE);
        this->Parameter(TextureParameter::eTextureWrapT, GL_CLAMP_TO_EDGE);
        //glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    template<GLuint type>
    void Texture<type>::WrapClampToBorder() {
        this->Parameter(TextureParameter::eTextureWrapS, GL_CLAMP_TO_BORDER);
        this->Parameter(TextureParameter::eTextureWrapT, GL_CLAMP_TO_BORDER);
        //glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        //glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
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

}
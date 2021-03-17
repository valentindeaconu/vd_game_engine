#include "Texture.hpp"

namespace vd::gl {

    void ActiveTexture(GLuint index) {
        glActiveTexture(GL_TEXTURE0 + index);
    }

    template class Texture<GL_TEXTURE_2D>;
    template class Texture<GL_TEXTURE_CUBE_MAP>;

    template<GLuint type>
    Texture<type>::Texture(size_t width, size_t height)
        : m_Dimension(width, height)
        , m_Id(0)
    {
    }

    template<GLuint T>
    void Texture<T>::Create() {
        glGenTextures(1, &m_Id);
    }

    template<GLuint T>
    void Texture<T>::CleanUp() {
        glDeleteTextures(1, &m_Id);
    }

    template<GLuint type>
    void Texture<type>::Bind() const {
        glBindTexture(type, m_Id);
    }

    template<GLuint type>
    void Texture<type>::BindToUnit(GLuint index) const {
        glActiveTexture(GL_TEXTURE0 + index);
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
    }

    template<GLuint type>
    void Texture<type>::LinearFilter() {
        this->Parameter(TextureParameter::eTextureMinFilter, GL_LINEAR);
        this->Parameter(TextureParameter::eTextureMagFilter, GL_LINEAR);
    }

    template<GLuint type>
    void Texture<type>::MipmapLinearFilter() {
        glGenerateMipmap(type);
        this->Parameter(TextureParameter::eTextureMinFilter, GL_LINEAR_MIPMAP_LINEAR);
        this->Parameter(TextureParameter::eTextureMagFilter, GL_LINEAR);
    }

    template<GLuint type>
    void Texture<type>::AnisotropyMipmapLinearFilter() {
        const GLfloat kMaxAnisotropy = 8.0f;
        GLfloat value;

        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &value);
        value = std::min(kMaxAnisotropy, value);

        this->Parameter(TextureParameter::eTextureLodBias, 0);

        glGenerateMipmap(type);
        this->Parameter(TextureParameter::eTextureMinFilter, GL_LINEAR_MIPMAP_LINEAR);
        this->Parameter(TextureParameter::eTextureMaxAnisotropy, value);
    }


    template<GLuint type>
    void Texture<type>::WrapRepeat() {
        this->Parameter(TextureParameter::eTextureWrapS, GL_REPEAT);
        this->Parameter(TextureParameter::eTextureWrapT, GL_REPEAT);

        if (GetDimension() == e3D) {
            this->Parameter(TextureParameter::eTextureWrapR, GL_REPEAT);
        }
    }

    template<GLuint type>
    void Texture<type>::WrapMirroredRepeat() {
        this->Parameter(TextureParameter::eTextureWrapS, GL_MIRRORED_REPEAT);
        this->Parameter(TextureParameter::eTextureWrapT, GL_MIRRORED_REPEAT);

        if (GetDimension() == e3D) {
            this->Parameter(TextureParameter::eTextureWrapR, GL_MIRRORED_REPEAT);
        }
    }

    template<GLuint type>
    void Texture<type>::WrapClampToEdge() {
        this->Parameter(TextureParameter::eTextureWrapS, GL_CLAMP_TO_EDGE);
        this->Parameter(TextureParameter::eTextureWrapT, GL_CLAMP_TO_EDGE);

        if (GetDimension() == e3D) {
            this->Parameter(TextureParameter::eTextureWrapR, GL_CLAMP_TO_EDGE);
        }
    }

    template<GLuint type>
    void Texture<type>::WrapClampToBorder() {
        this->Parameter(TextureParameter::eTextureWrapS, GL_CLAMP_TO_BORDER);
        this->Parameter(TextureParameter::eTextureWrapT, GL_CLAMP_TO_BORDER);

        if (GetDimension() == e3D) {
            this->Parameter(TextureParameter::eTextureWrapR, GL_CLAMP_TO_BORDER);
        }
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

    template<GLuint type>
    typename Texture<type>::DimensionType Texture<type>::GetDimension() const {
        switch (type) {
            case GL_TEXTURE_2D:
                return e2D;
            case GL_TEXTURE_CUBE_MAP:
                return e3D;
            default:
                return eNone;
        }
    }

}
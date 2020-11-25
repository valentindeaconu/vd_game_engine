//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_GL_TEXTURE_HPP
#define VD_GAME_ENGINE_GL_TEXTURE_HPP

#include <engine/loader/ImageLoader.hpp>

#include <engine/misc/Types.hpp>

#include <string>
#include <algorithm>

#include "GLTypes.hpp"

namespace vd::gl {
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

        void Parameter(const TextureParameter& parameter, int value);
        void Parameter(const TextureParameter& parameter, float value);
        void Parameter(const TextureParameter& parameter, const int* value);
        void Parameter(const TextureParameter& parameter, const float* value);

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
    private:
        GLuint m_Id;

        vd::Dimension m_Dimension;
    };

    typedef	Texture<GL_TEXTURE_2D>		Texture2D;
    typedef std::shared_ptr<Texture2D>	Texture2DPtr;
}

#endif // !__TEXTURE_HPP_

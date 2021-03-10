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
#include "GLComponent.hpp"

namespace vd::gl {
    void ActiveTexture(GLuint index);

    template<GLuint T>
    class Texture : public GLComponent {
    public:
        explicit Texture(size_t width = 0, size_t height = 0);

        void Create() override;
        void CleanUp() override;

        void Bind() const;
        void Unbind() const;

        void Parameter(const TextureParameter& parameter, int value);
        void Parameter(const TextureParameter& parameter, float value);
        void Parameter(const TextureParameter& parameter, const int* value);
        void Parameter(const TextureParameter& parameter, const float* value);

        void NoFilter();
        void LinearFilter();
        void MipmapLinearFilter();
        void AnisotropyMipmapLinearFilter();

        void WrapRepeat();
        void WrapMirroredRepeat();
        void WrapClampToEdge();
        void WrapClampToBorder();

        [[nodiscard]] GLuint Id() const;

        [[nodiscard]] size_t Width() const;
        [[nodiscard]] size_t Height() const;
    private:
        enum DimensionType {
            e1D = 0,
            e2D,
            e3D,
            eNone
        };
        DimensionType GetDimension() const;

        GLuint m_Id;

        vd::Dimension m_Dimension;
    };

    typedef	Texture<GL_TEXTURE_2D>		    Texture2D;
    typedef std::shared_ptr<Texture2D>	    Texture2DPtr;
    typedef Texture<GL_TEXTURE_CUBE_MAP>    TextureCubeMap;
    typedef std::shared_ptr<TextureCubeMap> TextureCubeMapPtr;

}

#endif // !__TEXTURE_HPP_

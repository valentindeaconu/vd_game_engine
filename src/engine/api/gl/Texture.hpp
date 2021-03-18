//
// Created by Vali on 11/11/2020.
//

#ifndef VDGE_GL_TEXTURE_HPP
#define VDGE_GL_TEXTURE_HPP

#include <engine/loader/ImageLoader.hpp>

#include <engine/defines/Types.hpp>

#include <string>
#include <algorithm>

#include "Enums.hpp"
#include "Component.hpp"
#include "Context.hpp"

namespace vd::gl {

    template<GLuint T>
    class Texture : public Component {
    public:
        explicit Texture(size_t width = 0, size_t height = 0);

        void Bind() const;
        void BindToUnit(GLuint index) const;
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
        void OnCreate() override;
        void OnCleanUp() override;

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

#endif //VDGE_GL_TEXTURE_HPP

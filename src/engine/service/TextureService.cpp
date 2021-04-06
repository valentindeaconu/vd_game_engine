//
// Created by Vali on 11/25/2020.
//

#include "TextureService.hpp"

namespace vd::service {
    namespace exception {
        TextureNameUsedException::TextureNameUsedException(const std::string& name)
            : CustomException("TextureNameUsedException", "Texture with name '" + name + "' is already in used")
        {
        }
    }

    void TextureService::CleanUp() {
        for (auto& t : s_2D_Collector) {
            t->CleanUp();
        }

        for (auto& t : s_CM_Collector) {
            t->CleanUp();
        }
    }

    gl::Texture2DPtr TextureService::UncachedCreateStorage(const Dimension& dimension,
                                                           const gl::TextureFormat& internalFormat,
                                                           int level) {
        gl::Texture2DPtr pTexture = std::make_shared<gl::Texture2D>(dimension.width, dimension.height);

        pTexture->Create();
        pTexture->Bind();

        glTexStorage2D(GL_TEXTURE_2D, level, internalFormat, dimension.width, dimension.height);

        pTexture->Unbind();

        s_2D_Collector.emplace_back(pTexture);

        return pTexture;
    }

    gl::Texture2DPtr TextureService::UncachedCreate(const Dimension& dimension,
                                                    const gl::TextureFormat& internalFormat,
                                                    const gl::TextureFormat& format,
                                                    const gl::DataType& type,
                                                    const void *data) {
        gl::Texture2DPtr pTexture = std::make_shared<gl::Texture2D>(dimension.width, dimension.height);

        pTexture->Create();
        pTexture->Bind();

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, dimension.width, dimension.height, 0, format, type, data);

        pTexture->Unbind();

        s_2D_Collector.emplace_back(pTexture);

        return pTexture;
    }

    gl::Texture2DPtr TextureService::Create(const std::string& name,
                                         const Dimension& dimension,
                                         const gl::TextureFormat& internalFormat,
                                         const gl::TextureFormat& format,
                                         const gl::DataType& type,
                                         const void *data) {
        if (s_2D_Cache.find(name) != s_2D_Cache.end()) {
            throw exception::TextureNameUsedException(name);
        }

        gl::Texture2DPtr pTexture = UncachedCreate(dimension, internalFormat, format, type, data);

        auto index = s_2D_Collector.size() - 1;

        s_2D_Cache[name] = index;

        return pTexture;
    }

    gl::Texture2DPtr TextureService::CreateFromFile(const std::string& path, size_t size, bool verticalFlip) {
        if (s_2D_Cache.find(path) != s_2D_Cache.end()) {
            return s_2D_Collector[s_2D_Cache[path]];
        }

        vd::model::ImagePtr<uint8_t> imagePtr = loader::ImageLoader::Load<uint8_t>(path, verticalFlip);

        gl::Texture2DPtr pTexture = std::make_shared<gl::Texture2D>(imagePtr->Width(), imagePtr->Height(), size);

        pTexture->Create();
        pTexture->Bind();

        // GL_SRGB, GL_RGBA
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_SRGB_ALPHA,
                     GLsizei(imagePtr->Width()),
                     GLsizei(imagePtr->Height()),
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     &(imagePtr->Data()[0]));

        pTexture->LinearFilter();
        pTexture->WrapRepeat();
        pTexture->Unbind();

        s_2D_Collector.emplace_back(pTexture);
        s_2D_Cache[path] = s_2D_Collector.size() - 1;

        return pTexture;
    }

    gl::TextureCubeMapPtr TextureService::CubeMapFromFiles(const CubeMapFacesPathVec& paths) {
        gl::TextureCubeMapPtr pTexture = std::make_shared<gl::TextureCubeMap>();

        pTexture->Create();
        pTexture->Bind();

        for (size_t i = 0; i < gl::g_kCubeMapFaceCount; ++i) {
            auto imagePtr = loader::ImageLoader::Load<uint8_t, vd::model::ImageFormat::eRGB>(paths[i], false);

            glTexImage2D(gl::CubeMapTextureFace::eRight + i,
                         0,
                         gl::eRGB,
                         imagePtr->Width(),
                         imagePtr->Height(),
                         0,
                         gl::eRGB,
                         gl::eUnsignedByte,
                         &(imagePtr->Data()[0]));
        }

        pTexture->LinearFilter();
        pTexture->WrapClampToEdge();
        pTexture->Parameter(gl::TextureParameter::eTextureWrapR, GL_CLAMP_TO_EDGE);

        pTexture->Unbind();

        s_CM_Collector.emplace_back(pTexture);

        return pTexture;
    }

    void TextureService::Remove(gl::Texture2DPtr& texture) {
        auto it = std::find_if(s_2D_Cache.begin(), s_2D_Cache.end(), [&](const auto& pair) {
            return s_2D_Collector[pair.second]->Id() == texture->Id();
        });

        if (it != s_2D_Cache.end()) {
            s_2D_Cache.erase(it);
        }

        texture = nullptr;
    }

}

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

    TextureService::TextureService() = default;

    TextureService& TextureService::getInstance() {
        static TextureService instance;
        return instance;
    }

    TextureService::~TextureService() {
        m_Cache.clear();
    }

    gl::Texture2DPtr TextureService::UncachedCreateStorage(const Dimension& dimension,
                                                           const gl::TextureFormat& internalFormat,
                                                           int level) {
        gl::Texture2DPtr pTexture = std::make_shared<gl::Texture2D>(dimension.width, dimension.height);

        pTexture->Generate();
        pTexture->Bind();

        glTexStorage2D(GL_TEXTURE_2D, level, internalFormat, dimension.width, dimension.height);

        pTexture->Unbind();

        return pTexture;
    }

    gl::Texture2DPtr TextureService::UncachedCreate(const Dimension& dimension,
                                                    const gl::TextureFormat& internalFormat,
                                                    const gl::TextureFormat& format,
                                                    const gl::DataType& type,
                                                    const void *data) {
        gl::Texture2DPtr pTexture = std::make_shared<gl::Texture2D>(dimension.width, dimension.height);

        pTexture->Generate();
        pTexture->Bind();

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, dimension.width, dimension.height, 0, format, type, data);

        pTexture->Unbind();

        return pTexture;
    }

    gl::Texture2DPtr TextureService::Create(const std::string& name,
                                         const Dimension& dimension,
                                         const gl::TextureFormat& internalFormat,
                                         const gl::TextureFormat& format,
                                         const gl::DataType& type,
                                         const void *data) {
        TextureService& instance = getInstance();

        if (instance.m_Cache.find(name) != instance.m_Cache.end()) {
            throw exception::TextureNameUsedException(name);
        }

        gl::Texture2DPtr pTexture = UncachedCreate(dimension, internalFormat, format, type, data);

        instance.m_Cache[name] = pTexture;

        return pTexture;
    }

    gl::Texture2DPtr TextureService::CreateFromFile(const std::string& path) {
        TextureService& instance = getInstance();

        if (instance.m_Cache.find(path) != instance.m_Cache.end()) {
            return instance.m_Cache[path];
        }

        vd::model::ImagePtr<uint8_t> imagePtr = loader::ImageLoader::Load<uint8_t>(path);

        gl::Texture2DPtr texture = std::make_shared<gl::Texture2D>(imagePtr);

        instance.m_Cache[path] = texture;

        return texture;
    }

    void TextureService::Remove(gl::Texture2DPtr& texture) {
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

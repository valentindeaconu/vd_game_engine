//
// Created by Vali on 11/25/2020.
//

#ifndef VD_GAME_ENGINE_TEXTURESERVICE_HPP
#define VD_GAME_ENGINE_TEXTURESERVICE_HPP

#include <engine/api/gl/Texture.hpp>
#include <engine/api/gl/Enums.hpp>
#include <engine/exception/Exceptions.hpp>

#include <algorithm>

namespace vd::service {
    namespace exception {
        struct TextureNameUsedException : public CustomException {
            explicit TextureNameUsedException(const std::string& name);
        };
    }

    class TextureService {
    public:
        typedef std::array<std::string, gl::g_kCubeMapFaceCount> CubeMapFacesPathVec;
        enum CubeMapFaceType {
            eRight = 0,
            eLeft,
            eTop,
            eBottom,
            eBack,
            eFront
        };

        static void CleanUp();

        static gl::Texture2DPtr UncachedCreateStorage(const vd::Dimension& dimension,
                                                      const gl::TextureFormat& internalFormat,
                                                      int level);

        static gl::Texture2DPtr UncachedCreate(const vd::Dimension& dimension,
                                               const gl::TextureFormat& internalFormat,
                                               const gl::TextureFormat& format,
                                               const gl::DataType& type,
                                               const void* data = nullptr);

        static gl::Texture2DPtr Create(const std::string& name,
                                       const vd::Dimension& dimension,
                                       const gl::TextureFormat& internalFormat,
                                       const gl::TextureFormat& format,
                                       const gl::DataType& type,
                                       const void* data = nullptr);

        static gl::Texture2DPtr CreateFromFile(const std::string& path);

        static gl::TextureCubeMapPtr CubeMapFromFiles(const CubeMapFacesPathVec& paths);

        static void Remove(gl::Texture2DPtr& texture);

    private:
        inline static std::unordered_map<std::string, size_t>  s_2D_Cache;
        inline static std::vector<gl::Texture2DPtr>            s_2D_Collector;

        inline static std::vector<gl::TextureCubeMapPtr>       s_CM_Collector;
    };
}


#endif //VD_GAME_ENGINE_TEXTURESERVICE_HPP

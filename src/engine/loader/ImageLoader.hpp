//
// Created by Vali on 11/13/2020.
//

#ifndef VD_GAME_ENGINE_IMAGELOADER_HPP
#define VD_GAME_ENGINE_IMAGELOADER_HPP

#include <engine/model/Image.hpp>

#include <engine/kernel/ObjectOfType.hpp>

namespace vd::loader {
    namespace impl {
        class IImageLoader {
        public:
            struct LoadedImage {
                uint8_t* data;
                size_t width, height;
                uint8_t channels;
            };

            virtual LoadedImage Load(const std::string& path) = 0;
            virtual void Release(const LoadedImage& loadedImage) = 0;
        };
        typedef std::shared_ptr<IImageLoader>   IImageLoaderPtr;
    }

    class ImageLoader {
    public:
        template <typename T, model::ImageFormat F = model::ImageFormat::eRGBA>
        static model::ImagePtr<T, F> Load(const std::string& path);
    };
}


#endif //VD_GAME_ENGINE_IMAGELOADER_HPP

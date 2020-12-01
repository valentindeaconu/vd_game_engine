//
// Created by Vali on 11/13/2020.
//

#include "StbiImpl.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace vd::loader::impl {

    IImageLoader::LoadedImage StbiImpl::Load(const std::string &path) {
        int x, y, n;
        int force_channels = 4;
        unsigned char* image_data = stbi_load(path.c_str(), &x, &y, &n, force_channels);
        if (!image_data) {
            throw std::runtime_error("could not load image from " + path);
        }

        if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
            vd::Logger::log("Texture " + path + " is not power-of-2 dimension");
        }

        LoadedImage image{};
        image.data = image_data;
        image.width = x;
        image.height = y;
        image.channels = force_channels;

        return image;
    }

    void StbiImpl::Release(const IImageLoader::LoadedImage& loadedImage) {
        stbi_image_free(loadedImage.data);
    }
}
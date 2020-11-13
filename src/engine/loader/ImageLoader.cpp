//
// Created by Vali on 11/13/2020.
//

#include "ImageLoader.hpp"

namespace vd::loader {
    /// Float data, RGBA format (4 channels)
    template<>
    model::ImagePtr<float, model::ImageFormat::eRGBA> ImageLoader::Load(const std::string& path) {
        impl::IImageLoaderPtr impl = vd::ObjectOfType<impl::IImageLoader>::Find();

        impl::IImageLoader::LoadedImage loadedImage = impl->Load(path);


        model::ImagePtr<float, model::ImageFormat::eRGBA> imagePtr =
                std::make_shared<model::Image<float, model::ImageFormat::eRGBA>>(loadedImage.width, loadedImage.height);

        uint64_t len = loadedImage.width * loadedImage.height * 4;
        imagePtr->Data().reserve(len);

        for (size_t i = 0; i < len; i += 4) {
            imagePtr->Data().emplace_back((float)loadedImage.data[i] / 255.0f);       // R
            imagePtr->Data().emplace_back((float)loadedImage.data[i + 1] / 255.0f);   // G
            imagePtr->Data().emplace_back((float)loadedImage.data[i + 2] / 255.0f);   // B
            imagePtr->Data().emplace_back((float)loadedImage.data[i + 3] / 255.0f);   // A
        }

        imagePtr->Reverse();

        impl->Release(loadedImage);

        return imagePtr;
    }

    /// Float data, BGRA format (4 channels)
    template<>
    model::ImagePtr<float, model::ImageFormat::eBGRA> ImageLoader::Load(const std::string& path) {
        impl::IImageLoaderPtr impl = vd::ObjectOfType<impl::IImageLoader>::Find();

        impl::IImageLoader::LoadedImage loadedImage = impl->Load(path);


        model::ImagePtr<float, model::ImageFormat::eBGRA> imagePtr =
                std::make_shared<model::Image<float, model::ImageFormat::eBGRA>>(loadedImage.width, loadedImage.height);

        uint64_t len = loadedImage.width * loadedImage.height * 4;
        imagePtr->Data().reserve(len);

        for (size_t i = 0; i < len; i += 4) {
            imagePtr->Data().emplace_back((float)loadedImage.data[i + 2] / 255.0f);   // B
            imagePtr->Data().emplace_back((float)loadedImage.data[i + 1] / 255.0f);   // G
            imagePtr->Data().emplace_back((float)loadedImage.data[i] / 255.0f);       // R
            imagePtr->Data().emplace_back((float)loadedImage.data[i + 3] / 255.0f);   // A
        }

        imagePtr->Reverse();

        impl->Release(loadedImage);

        return imagePtr;
    }

    /// Float data, RGB format (3 channels)
    template<>
    model::ImagePtr<float, model::ImageFormat::eRGB> ImageLoader::Load(const std::string& path) {
        impl::IImageLoaderPtr impl = vd::ObjectOfType<impl::IImageLoader>::Find();

        impl::IImageLoader::LoadedImage loadedImage = impl->Load(path);

        model::ImagePtr<float, model::ImageFormat::eRGB> imagePtr =
                std::make_shared<model::Image<float, model::ImageFormat::eRGB>>(loadedImage.width, loadedImage.height);

        uint64_t len = loadedImage.width * loadedImage.height * 4;
        imagePtr->Data().reserve(loadedImage.width * loadedImage.height * 3);

        for (size_t i = 0; i < len; i += 4) {
            imagePtr->Data().emplace_back((float)loadedImage.data[i] / 255.0f);       // R
            imagePtr->Data().emplace_back((float)loadedImage.data[i + 1] / 255.0f);   // G
            imagePtr->Data().emplace_back((float)loadedImage.data[i + 2] / 255.0f);   // B
        }

        imagePtr->Reverse();

        impl->Release(loadedImage);

        return imagePtr;
    }

    /// Float data, BGR format (3 channels)
    template<>
    model::ImagePtr<float, model::ImageFormat::eBGR> ImageLoader::Load(const std::string& path) {
        impl::IImageLoaderPtr impl = vd::ObjectOfType<impl::IImageLoader>::Find();

        impl::IImageLoader::LoadedImage loadedImage = impl->Load(path);

        model::ImagePtr<float, model::ImageFormat::eBGR> imagePtr =
                std::make_shared<model::Image<float, model::ImageFormat::eBGR>>(loadedImage.width, loadedImage.height);

        uint64_t len = loadedImage.width * loadedImage.height * 4;
        imagePtr->Data().reserve(loadedImage.width * loadedImage.height * 3);

        for (size_t i = 0; i < len; i += 4) {
            imagePtr->Data().emplace_back((float)loadedImage.data[i + 2] / 255.0f);   // B
            imagePtr->Data().emplace_back((float)loadedImage.data[i + 1] / 255.0f);   // G
            imagePtr->Data().emplace_back((float)loadedImage.data[i] / 255.0f);       // R
        }

        imagePtr->Reverse();

        impl->Release(loadedImage);

        return imagePtr;
    }

    /// Float data, R format (1 channel)
    template<>
    model::ImagePtr<float, model::ImageFormat::eR> ImageLoader::Load(const std::string& path) {
        impl::IImageLoaderPtr impl = vd::ObjectOfType<impl::IImageLoader>::Find();

        impl::IImageLoader::LoadedImage loadedImage = impl->Load(path);

        model::ImagePtr<float, model::ImageFormat::eR> imagePtr =
                std::make_shared<model::Image<float, model::ImageFormat::eR>>(loadedImage.width, loadedImage.height);

        uint64_t len = loadedImage.width * loadedImage.height * 4;
        imagePtr->Data().reserve(loadedImage.width * loadedImage.height);

        for (size_t i = 0; i < len; i += 4) {
            imagePtr->Data().emplace_back((float)loadedImage.data[i] / 255.0f);       // R
        }

        imagePtr->Reverse();

        impl->Release(loadedImage);

        return imagePtr;
    }

    /// Byte data, RGBA format (4 channels)
    template<>
    model::ImagePtr<uint8_t, model::ImageFormat::eRGBA> ImageLoader::Load(const std::string& path) {
        impl::IImageLoaderPtr impl = vd::ObjectOfType<impl::IImageLoader>::Find();

        impl::IImageLoader::LoadedImage loadedImage = impl->Load(path);

        model::ImagePtr<uint8_t, model::ImageFormat::eRGBA> imagePtr =
                std::make_shared<model::Image<uint8_t, model::ImageFormat::eRGBA>>(loadedImage.width, loadedImage.height);

        uint64_t len = loadedImage.width * loadedImage.height * 4;
        imagePtr->Data().reserve(len);

        for (size_t i = 0; i < len; i += 4) {
            imagePtr->Data().emplace_back(loadedImage.data[i]);       // R
            imagePtr->Data().emplace_back(loadedImage.data[i + 1]);   // G
            imagePtr->Data().emplace_back(loadedImage.data[i + 2]);   // B
            imagePtr->Data().emplace_back(loadedImage.data[i + 3]);   // A
        }

        imagePtr->Reverse();

        impl->Release(loadedImage);

        return imagePtr;
    }

    /// Byte data, BGRA format (4 channels)
    template<>
    model::ImagePtr<uint8_t, model::ImageFormat::eBGRA> ImageLoader::Load(const std::string& path) {
        impl::IImageLoaderPtr impl = vd::ObjectOfType<impl::IImageLoader>::Find();

        impl::IImageLoader::LoadedImage loadedImage = impl->Load(path);

        model::ImagePtr<uint8_t, model::ImageFormat::eBGRA> imagePtr =
                std::make_shared<model::Image<uint8_t, model::ImageFormat::eBGRA>>(loadedImage.width, loadedImage.height);

        uint64_t len = loadedImage.width * loadedImage.height * 4;
        imagePtr->Data().reserve(len);

        for (size_t i = 0; i < len; i += 4) {
            imagePtr->Data().emplace_back(loadedImage.data[i + 2]);   // B
            imagePtr->Data().emplace_back(loadedImage.data[i + 1]);   // G
            imagePtr->Data().emplace_back(loadedImage.data[i]);       // R
            imagePtr->Data().emplace_back(loadedImage.data[i + 3]);   // A
        }

        imagePtr->Reverse();

        impl->Release(loadedImage);

        return imagePtr;
    }

    /// Byte data, RGB format (3 channels)
    template<>
    model::ImagePtr<uint8_t, model::ImageFormat::eRGB> ImageLoader::Load(const std::string& path) {
        impl::IImageLoaderPtr impl = vd::ObjectOfType<impl::IImageLoader>::Find();

        impl::IImageLoader::LoadedImage loadedImage = impl->Load(path);

        model::ImagePtr<uint8_t, model::ImageFormat::eRGB> imagePtr =
                std::make_shared<model::Image<uint8_t, model::ImageFormat::eRGB>>(loadedImage.width, loadedImage.height);

        uint64_t len = loadedImage.width * loadedImage.height * 4;
        imagePtr->Data().reserve(loadedImage.width * loadedImage.height * 3);

        for (size_t i = 0; i < len; i += 4) {
            imagePtr->Data().emplace_back(loadedImage.data[i]);       // R
            imagePtr->Data().emplace_back(loadedImage.data[i + 1]);   // G
            imagePtr->Data().emplace_back(loadedImage.data[i + 2]);   // B
        }

        imagePtr->Reverse();

        impl->Release(loadedImage);

        return imagePtr;
    }

    /// Byte data, BGR format (3 channels)
    template<>
    model::ImagePtr<uint8_t, model::ImageFormat::eBGR> ImageLoader::Load(const std::string& path) {
        impl::IImageLoaderPtr impl = vd::ObjectOfType<impl::IImageLoader>::Find();

        impl::IImageLoader::LoadedImage loadedImage = impl->Load(path);

        model::ImagePtr<uint8_t, model::ImageFormat::eBGR> imagePtr =
                std::make_shared<model::Image<uint8_t, model::ImageFormat::eBGR>>(loadedImage.width, loadedImage.height);

        uint64_t len = loadedImage.width * loadedImage.height * 4;
        imagePtr->Data().reserve(loadedImage.width * loadedImage.height * 3);

        for (size_t i = 0; i < len; i += 4) {
            imagePtr->Data().emplace_back(loadedImage.data[i + 2]);   // B
            imagePtr->Data().emplace_back(loadedImage.data[i + 1]);   // G
            imagePtr->Data().emplace_back(loadedImage.data[i]);       // R
        }

        imagePtr->Reverse();

        impl->Release(loadedImage);

        return imagePtr;
    }

    /// Byte data, R format (1 channel)
    template<>
    model::ImagePtr<uint8_t, model::ImageFormat::eR> ImageLoader::Load(const std::string& path) {
        impl::IImageLoaderPtr impl = vd::ObjectOfType<impl::IImageLoader>::Find();

        impl::IImageLoader::LoadedImage loadedImage = impl->Load(path);

        model::ImagePtr<uint8_t, model::ImageFormat::eR> imagePtr =
                std::make_shared<model::Image<uint8_t, model::ImageFormat::eR>>(loadedImage.width, loadedImage.height);

        uint64_t len = loadedImage.width * loadedImage.height * 4;
        imagePtr->Data().reserve(loadedImage.width * loadedImage.height);

        for (size_t i = 0; i < len; i += 4) {
            imagePtr->Data().emplace_back(loadedImage.data[i]);       // R
        }

        imagePtr->Reverse();

        impl->Release(loadedImage);

        return imagePtr;
    }
}
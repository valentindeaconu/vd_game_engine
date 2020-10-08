#ifndef __IMAGE_HPP_
#define __IMAGE_HPP_

#include <cstdint>
#include <vector>
#include <memory>
#include <cassert>

#include <engine/logger/Logger.hpp>

namespace vd::img {
    template<typename T>
    struct Pixel
    {
        T r;
        T g;
        T b;
        T a;

        Pixel(T r, T g, T b, T a);
        Pixel();

        template<typename E> Pixel<E> cast() const;
    };

    typedef Pixel<uint8_t>	PixelB;
    typedef Pixel<float>	PixelF;
    typedef Pixel<uint16_t>	PixelS;

    enum PixelFormat
    {
        eRGBA = 0,
        eBGRA
    };

    template<typename T>
    class RawImage {
    public:
        size_t width;
        size_t height;
        std::vector<T> data;
    };
    typedef RawImage<float> RawFloatImage;
    typedef std::shared_ptr<RawFloatImage>  RawFloatImagePtr;

    template<typename T>
    class Image
    {
    public:
        Image(size_t width, size_t height);
        Image(Image<T>& other);
        ~Image();

        void expand(const Pixel<T>& pixel);
        void reverse();

        template<typename E> Image<E> cast() const;

        Pixel<T> at(size_t i, size_t j, const PixelFormat& format = PixelFormat::eBGRA) const;

        const size_t width;
        const size_t height;
        std::vector<T> data;

    private:
        int iterator;
    };
    typedef Image<uint8_t>	ImageB;
    typedef Image<float>	ImageF;
    typedef Image<uint16_t>	ImageS;

    typedef std::shared_ptr<ImageB>	ImageBPtr;
    typedef std::shared_ptr<ImageF>	ImageFPtr;
    typedef std::shared_ptr<ImageS>	ImageSPtr;
}

#endif // !__IMAGE_HPP_

#include "IIMGLoader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace vd::img::internal::impl {
    /*ImageBPtr OpenCVIMGLoader::loadByteImage(const std::string& path)
    {
        cv::Mat img = cv::imread(path, CV_LOAD_IMAGE_UNCHANGED);

        ImageBPtr imagePtr = std::make_shared<ImageB>(img.cols, img.rows);

        size_t channels = img.channels();

        for (int i = 0; i < img.rows; ++i)
        {
            for (int j = 0; j < img.cols; ++j)
            {
                switch (channels)
                {
                    case 1:
                    {
                        uchar g = img.at<uchar>(i, j);
                        PixelB pixel(g, g, g, 255);
                        imagePtr->expand(pixel);
                        break;
                    }
                    case 3:
                    {
                        cv::Vec3b pxl = img.at<cv::Vec3b>(i, j);
                        PixelB pixel(pxl[2], pxl[1], pxl[0], 255);
                        imagePtr->expand(pixel);
                        break;
                    }
                    case 4:
                    {
                        cv::Vec4b pxl = img.at<cv::Vec4b>(i, j);
                        PixelB pixel(pxl[2], pxl[1], pxl[0], pxl[3]);
                        imagePtr->expand(pixel);
                        break;
                    }
                    default:
                    {
                        PixelB pixel(0, 0, 0, 255);
                        imagePtr->expand(pixel);
                        break;
                    }
                }
            }
        }

        imagePtr->reverse();

        return imagePtr;
    }

    ImageFPtr OpenCVIMGLoader::loadFloatImage(const std::string& path)
    {
        cv::Mat img = cv::imread(path, CV_LOAD_IMAGE_UNCHANGED);

        ImageFPtr imagePtr = std::make_shared<ImageF>(img.cols, img.rows);

        size_t channels = img.channels();

        for (int i = 0; i < img.rows; ++i)
        {
            for (int j = 0; j < img.cols; ++j)
            {
                switch (channels)
                {
                case 1:
                {
                    uchar g = img.at<uchar>(i, j);
                    float g_f = ((float)g / 255.0f);
                    PixelF pixel(g_f, g_f, g_f, 1.0f);
                    imagePtr->expand(pixel);
                    break;
                }
                case 3:
                {
                    cv::Vec3b pxl = img.at<cv::Vec3b>(i, j);
                    float r = ((float)pxl[2] / 255.0f);
                    float g = ((float)pxl[1] / 255.0f);
                    float b = ((float)pxl[0] / 255.0f);
                    PixelF pixel(r, g, b, 1.0f);
                    imagePtr->expand(pixel);
                    break;
                }
                case 4:
                {
                    cv::Vec4b pxl = img.at<cv::Vec4b>(i, j);
                    float r = ((float)pxl[2] / 255.0f);
                    float g = ((float)pxl[1] / 255.0f);
                    float b = ((float)pxl[0] / 255.0f);
                    float a = ((float)pxl[3] / 255.0f);
                    PixelF pixel(r, g, b, a);
                    imagePtr->expand(pixel);
                    break;
                }
                default:
                {
                    PixelF pixel(0.0f, 0.0f, 0.0f, 1.0f);
                    imagePtr->expand(pixel);
                    break;
                }
                }
            }
        }

        imagePtr->reverse();

        return imagePtr;
    }*/

    ImageBPtr stbiIMGLoader::loadByteImage(const std::string& path)
    {
        int x, y, n;
        int force_channels = 4;
        unsigned char* image_data = stbi_load(path.c_str(), &x, &y, &n, force_channels);
        if (!image_data)
        {
            vd::Logger::log("Could not load " + path);
            return nullptr;
        }


        if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0)
        {
            vd::Logger::log("Texture " + path + " is not power-of-2 dimension");
        }

        ImageB _img(x, y);

        size_t len = x * y * 4;
        for (size_t i = 0; i < len; i += 4)
        {
            PixelB pixel(image_data[i], image_data[i + 1], image_data[i + 2], image_data[i + 3]);
            _img.expand(pixel);
        }
        stbi_image_free(image_data);

        ImageBPtr imagePtr = std::make_shared<ImageB>(_img);

        imagePtr->reverse();

        return imagePtr;
    }

    ImageFPtr stbiIMGLoader::loadFloatImage(const std::string& path)
    {
        int x, y, n;
        int force_channels = 4;
        unsigned char* image_data = stbi_load(path.c_str(), &x, &y, &n, force_channels);
        if (!image_data)
        {
            vd::Logger::warn("Could not load " + path);
            return nullptr;
        }


        if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0)
        {
            vd::Logger::log("Texture " + path + " is not power-of-2 dimension");
        }

        ImageF _img(x, y);

        size_t len = x * y * 4;
        for (size_t i = 0; i < len; i += 4)
        {
            float r = ((float)image_data[i] / 255.0f);
            float g = ((float)image_data[i + 1] / 255.0f);
            float b = ((float)image_data[i + 2] / 255.0f);
            float a = ((float)image_data[i + 3] / 255.0f);
            PixelF pixel(r, g, b, a);
            _img.expand(pixel);
        }
        stbi_image_free(image_data);

        ImageFPtr imagePtr = std::make_shared<ImageF>(_img);

        imagePtr->reverse();

        return imagePtr;
    }

    RawFloatImagePtr stbiIMGLoader::loadRawFloatImage(const std::string &path) {
        int x, y, n;
        int force_channels = 4;
        unsigned char* image_data = stbi_load(path.c_str(), &x, &y, &n, force_channels);
        if (!image_data)
        {
            vd::Logger::warn("Could not load " + path);
            return nullptr;
        }


        if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0)
        {
            vd::Logger::log("Texture " + path + " is not power-of-2 dimension");
        }

        int width_in_bytes = (x << 2);

        unsigned char* top = NULL;
        unsigned char* bottom = NULL;
        unsigned char temp = 0;

        int half_height = (y >> 1);

        for (int row = 0; row < half_height; row++) {
            top = image_data + row * width_in_bytes;
            bottom = image_data + (x - row - 1) * width_in_bytes;
            for (int col = 0; col < width_in_bytes; col++) {
                temp = *top;
                *top = *bottom;
                *bottom = temp;
                top++;
                bottom++;
            }
        }

        RawFloatImagePtr rawFloatImagePtr = std::make_shared<RawFloatImage>();
        rawFloatImagePtr->width = x;
        rawFloatImagePtr->height = y;
        rawFloatImagePtr->data.reserve(x * y);

        size_t len = x * y * 4;
        for (size_t i = 0; i < len; i += 4) {
            rawFloatImagePtr->data.emplace_back(float(image_data[i]) / 255.0f);
        }

        stbi_image_free(image_data);

        return rawFloatImagePtr;
    }
}
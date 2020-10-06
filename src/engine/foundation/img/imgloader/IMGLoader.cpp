#include "IMGLoader.hpp"

namespace vd::img {
    IMGLoader::IMGLoader()
    {
        loaderImplPtr = std::make_shared<IMGLoaderImpl>();
    }

    IMGLoader::~IMGLoader()
    {
        loaderImplPtr = nullptr;
    }

    ImageBPtr IMGLoader::loadByteImage(const std::string& path)
    {
        return loaderImplPtr->loadByteImage(path);
    }

    ImageFPtr IMGLoader::loadFloatImage(const std::string& path)
    {
        return loaderImplPtr->loadFloatImage(path);
    }

    void IMGLoader::showByteImage(const ImageB& image)
    {
        // TODO: Show image with stbi

        /* cv::Mat img(image.width, image.height, CV_8UC4);
        for (size_t i = 0; i < img.rows; ++i)
        {
            for (size_t j = 0; j < img.cols; ++j)
            {
                PixelB pixel = image.at(i, j);
                img.at<cv::Vec4b>(i, j) = cv::Vec4b(pixel.r, pixel.g, pixel.b, pixel.a);
            }
        }

        //static int x = 0;
        //std::stringstream ss;
        //ss << "Test " << x++;
        //cv::imshow(ss.str(), img);*/
    }
}
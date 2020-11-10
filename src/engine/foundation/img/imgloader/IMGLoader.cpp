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

    RawFloatImagePtr IMGLoader::loadRawFloatImage(const std::string &path) {
        return loaderImplPtr->loadRawFloatImage(path);
    }
}
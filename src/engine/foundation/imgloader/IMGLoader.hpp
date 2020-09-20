#ifndef __IMG_LOADER_HPP_
#define __IMG_LOADER_HPP_

#include <string>
#include <cstdint>
#include <unordered_map>
#include <memory>

#include "IIMGLoader.hpp"
#include "model/Image.hpp"

namespace vd::imgloader
{
    class IMGLoader
    {
    public:
        IMGLoader();
        ~IMGLoader();

        ImageBPtr loadByteImage(const std::string& path);
        ImageFPtr loadFloatImage(const std::string& path);

        // Debugging purposes
        static void showByteImage(const ImageB& imagePtr);
    private:
        /// Change Impl here
        // typedef internal::impl::OpenCVIMGLoader	IMGLoaderImpl;
        typedef internal::impl::stbiIMGLoader	IMGLoaderImpl;

        internal::IIMGLoaderPtr loaderImplPtr;
    };
    typedef std::shared_ptr<IMGLoader>	IMGLoaderPtr;
}

#endif // !__IMGLOADER_HPP_

#ifndef __IMG_LOADER_HPP_
#define __IMG_LOADER_HPP_

#include <string>
#include <cstdint>
#include <unordered_map>
#include <memory>

#include "IIMGLoader.hpp"
#include "engine/foundation/img/Image.hpp"

namespace vd::img {
    class IMGLoader
    {
    public:
        IMGLoader();
        ~IMGLoader();

        ImageBPtr loadByteImage(const std::string& path);
        ImageFPtr loadFloatImage(const std::string& path);
        RawFloatImagePtr loadRawFloatImage(const std::string& path);
    private:
        /// Change Impl here
        // typedef internal::impl::OpenCVIMGLoader	IMGLoaderImpl;
        typedef internal::impl::stbiIMGLoader	IMGLoaderImpl;

        internal::IIMGLoaderPtr loaderImplPtr;
    };
    typedef std::shared_ptr<IMGLoader>	IMGLoaderPtr;
}

#endif // !__IMGLOADER_HPP_

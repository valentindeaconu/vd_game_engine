#ifndef __IIMG_LOADER_HPP_
#define __IIMG_LOADER_HPP_

#include "model/Image.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "stb_image.h"

namespace vd
{
	namespace imgloader
	{
		namespace internal
		{
			class IIMGLoader
			{
			public:
				virtual ImageBPtr loadByteImage(const std::string& path) = 0;
				virtual ImageFPtr loadFloatImage(const std::string& path) = 0;
			};
			typedef std::shared_ptr<IIMGLoader>	IIMGLoaderPtr;


			namespace impl
			{
				class OpenCVIMGLoader : public IIMGLoader
				{
				public:
					ImageBPtr loadByteImage(const std::string& path);
					ImageFPtr loadFloatImage(const std::string& path);
				};
				typedef std::shared_ptr<OpenCVIMGLoader>	OpenCVIMGLoaderPtr;

				class stbiIMGLoader : public IIMGLoader
				{
				public:
					ImageBPtr loadByteImage(const std::string& path);
					ImageFPtr loadFloatImage(const std::string& path);
				};
				typedef std::shared_ptr<stbiIMGLoader>		stbiIMGLoaderPtr;
			}
		}		
	}
}

#endif // !__IIMG_LOADER_HPP_

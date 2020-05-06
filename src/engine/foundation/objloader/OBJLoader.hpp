#pragma once

#ifndef __OBJLOADER_HPP_
#define __OBJLOADER_HPP_

#include <unordered_set>
#include <thread>
#include <cstdint>
#include <future>
#include <mutex>

#include <engine/model/Mesh.hpp>

namespace vd
{
	namespace objloader
	{
		class OBJLoader
		{
		public:
			OBJLoader();
			~OBJLoader();
			
			void load(const std::string& basePath, const std::string& objFile, vd::model::MeshPtrVec& meshes);
		private:
		};
		typedef std::shared_ptr<OBJLoader>	OBJLoaderPtr;
	}
}

#endif // !__OBJLOADER_HPP_

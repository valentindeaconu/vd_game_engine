#ifndef __BUFFER_HPP_
#define __BUFFER_HPP_

#include <engine/model/Mesh.hpp>

namespace vd
{
	namespace buffer
	{
		class Buffer
		{
		public:
			virtual void allocate(const vd::model::MeshPtr& meshPtr) = 0;
			virtual void render() = 0;
			virtual void cleanUp() = 0;
		};
		typedef std::shared_ptr<Buffer>	BufferPtr;
		typedef std::vector<BufferPtr>	BufferPtrVec;
	}
}

#endif // !__BUFFER_HPP_

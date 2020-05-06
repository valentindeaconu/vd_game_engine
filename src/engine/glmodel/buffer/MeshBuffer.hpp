#ifndef __MESH_BUFFER_HPP_
#define __MESH_BUFFER_HPP_

#include <engine/foundation/GL.hpp>

#include <engine/model/Mesh.hpp>
#include <engine/model/Texture.hpp>

#include "Buffer.hpp"

namespace vd
{
	namespace buffer
	{
		class MeshBuffer : public Buffer
		{
		public:
			MeshBuffer();
			~MeshBuffer();

			void allocate(const vd::model::MeshPtr& meshPtr);
			void render();
			void cleanUp();
		private:
			GLuint vaoId;
			GLuint vbId;
			GLuint ebId;

			vd::model::MeshPtr meshPtr;
		};
		typedef std::shared_ptr<MeshBuffer>	MeshBufferPtr;
		typedef std::vector<MeshBufferPtr>	MeshBufferPtrVec;
	}
}

#endif // !__MESH_BUFFER_HPP_

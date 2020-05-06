#ifndef __MESH_HPP_
#define __MESH_HPP_

#include <engine/foundation/GL.hpp>

#include <string>
#include <vector>

#include "Vertex.hpp"
#include "Material.hpp"

namespace vd 
{
	namespace model
	{
		struct Mesh
		{
			std::vector<Vertex>	vertices;
			std::vector<GLuint>	indices;
			std::vector<Material> materials;
		};
		typedef std::shared_ptr<Mesh>	MeshPtr;
		typedef std::vector<MeshPtr>	MeshPtrVec;
	}
}
#endif // !__MESH_HPP_

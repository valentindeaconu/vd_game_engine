#ifndef __MATERIAL_HPP_
#define __MATERIAL_HPP_

#include <glm/glm.hpp>

#include <string>

#include "Texture.hpp"

namespace vd
{
	namespace model
	{
		struct Material
		{
			std::string name;
			Texture2DPtr diffusemap;
			Texture2DPtr normalmap;
			Texture2DPtr displacemap;
			Texture2DPtr ambientmap;
			Texture2DPtr specularmap;
			Texture2DPtr alphamap;
			Texture2DPtr bumpmap;
			glm::vec4 color;
			glm::vec3 emission;
			float shininess;
			float displaceScale;
			float horizontalScale;
		};
	}
}

#endif // !__MATERIAL_HPP_

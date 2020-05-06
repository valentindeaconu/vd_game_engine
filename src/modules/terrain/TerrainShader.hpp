#ifndef __TERRAIN_SHADER_HPP_
#define __TERRAIN_SHADER_HPP_

#include <engine/shader/Shader.hpp>

#include "Terrain.hpp"

namespace mod
{
	namespace terrain
	{
		class TerrainShader : public vd::shader::Shader
		{
		public:
			TerrainShader();
			~TerrainShader();

			void updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex);
		private:
			const int kMaxTextures;
			const int kMaxLights;

			std::vector<std::string> texturePackUniformNames;
		};
		typedef std::shared_ptr<TerrainShader>	TerrainShaderPtr;
	}
}

#endif // !__TERRAIN_SHADER_HPP_
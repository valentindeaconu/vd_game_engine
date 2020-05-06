#ifndef __PLAYER_SHADER_HPP_
#define __PLAYER_SHADER_HPP_

#include <engine/shader/Shader.hpp>

namespace mod
{
	namespace player
	{
		class PlayerShader : public vd::shader::Shader
		{
		public:
			PlayerShader();
			~PlayerShader();

			void updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex);
		private:
			const size_t kMaxLights;
		};
		typedef std::shared_ptr<PlayerShader>	PlayerShaderPtr;
	}
}

#endif // !__PLAYER_SHADER_HPP_
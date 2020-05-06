#ifndef __STATIC_OBJECT_SHADER_HPP_
#define __STATIC_OBJECT_SHADER_HPP_

#include <engine/shader/Shader.hpp>

namespace mod
{
	namespace sobj
	{
		class StaticObjectShader : public vd::shader::Shader
		{
		public:
			StaticObjectShader();
			~StaticObjectShader();

			void updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex);
		private:
			const int kMaxLights;
		};
		typedef std::shared_ptr<StaticObjectShader>	StaticObjectShaderPtr;
	}
}

#endif // !__STATIC_OBJECT_SHADER_HPP_

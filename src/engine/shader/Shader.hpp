#ifndef __SHADER_HPP_
#define __SHADER_HPP_

#include <engine/foundation/GL.hpp>
#include <engine/object/Entity.hpp>

#include <vector>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "GLSLWrapper.hpp"

namespace vd
{
	namespace shader
	{
		enum ShaderType
		{
			eVertexShader = GL_VERTEX_SHADER,
			eGeometryShader = GL_GEOMETRY_SHADER,
			eFragmentShader = GL_FRAGMENT_SHADER,
			eTesselationControlShader = GL_TESS_CONTROL_SHADER,
			eTesselationEvaluationShader = GL_TESS_EVALUATION_SHADER,
			eComputeShader = GL_COMPUTE_SHADER
		};

		/*class ShaderLoader
		{
		public:
			static ShaderLoader& getInstance();

			void load(const std::string& path, std::string& shaderProgram);

		private:
			static ShaderLoader instance;
		};*/

		class Shader
		{
		public:
			Shader();

			void bind() const;

			void addUniform(const std::string& uniformName);
			void addUniformBlock(const std::string& uniformBlockName);

			void loadAndAddShader(const std::string& path, uint32_t type);
			void addShader(const std::string& shaderProgram, uint32_t type);
			void compileShader();

			void setUniformi(const std::string& uniformName, int32_t value) const;
			void setUniformf(const std::string& uniformName, float value) const;
			void setUniform(const std::string& uniformName, const glm::vec2& value) const;
			void setUniform(const std::string& uniformName, const glm::vec3& value) const;
			void setUniform(const std::string& uniformName, const glm::quat& value) const;
			void setUniform(const std::string& uniformName, const glm::mat3& value) const;
			void setUniform(const std::string& uniformName, const glm::mat4& value) const;

			void bindUniformBlock(const std::string& uniformBlockName, uint32_t uniformBlockBinding) const;
			void bindFragDataLocation(const std::string& name, uint32_t index) const;

			uint32_t getProgram() const;

			virtual void updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) = 0;
		private:
			GLSLWrapper glslWrapper;

			uint32_t program;
			std::unordered_map<std::string, uint32_t> uniforms;
		};
		typedef std::shared_ptr<Shader>	ShaderPtr;
	}
}

#endif // !__SHADER_HPP_

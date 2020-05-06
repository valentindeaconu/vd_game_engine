#include "SkyShader.hpp"

namespace mod
{
	namespace sky
	{
		SkyShader::SkyShader()
			: vd::shader::Shader()
		{
			loadAndAddShader("./resources/shaders/sky/sky_VS.glsl", vd::shader::eVertexShader);
			loadAndAddShader("./resources/shaders/sky/sky_FS.glsl", vd::shader::eFragmentShader);
			compileShader();

			addUniform("model");
			addUniform("view");
			addUniform("projection");
			
			addUniform("fogDensity");
			addUniform("fogGradient");
			addUniform("fogColor");
		}

		SkyShader::~SkyShader()
		{

		}

		void SkyShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex)
		{
			setUniform("model", entityPtr->getWorldTransform().get());

			auto& enginePtr = entityPtr->getParentEngine();
			setUniform("view", enginePtr->getCamera()->getViewMatrix());
			setUniform("projection", enginePtr->getWindow()->getProjectionMatrix());

			static bool loadedBasics = false;
			if (loadedBasics == false)
			{
				auto& engineConfigPtr = enginePtr->getEngineConfig();
				setUniformf("fogDensity", engineConfigPtr->getFogDensity());
				setUniformf("fogGradient", engineConfigPtr->getFogGradient());
				setUniform("fogColor", engineConfigPtr->getFogColor());
				loadedBasics = true;
			}
		}
	}
}
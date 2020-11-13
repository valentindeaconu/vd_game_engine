#ifndef __SKY_SHADER_HPP_
#define __SKY_SHADER_HPP_

#include <engine/shader/Shader.hpp>

#include <engine/kernel/ObjectOfType.hpp>
#include <engine/misc/Properties.hpp>
#include <engine/kernel/Engine.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/window/Window.hpp>

namespace mod::sky {
    class SkyShader : public vd::shader::Shader {
    public:
        SkyShader();
        ~SkyShader();

        void updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) override;
    };
    typedef std::shared_ptr<SkyShader>	SkyShaderPtr;
}

#endif // !__SKY_SHADER_HPP_
#ifndef __SKY_SHADER_HPP_
#define __SKY_SHADER_HPP_

#include <engine/shader/Shader.hpp>

namespace mod::sky
{
    class SkyShader : public vd::shader::Shader
    {
    public:
        SkyShader();
        ~SkyShader();

        void updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) override;
    };
    typedef std::shared_ptr<SkyShader>	SkyShaderPtr;
}

#endif // !__SKY_SHADER_HPP_
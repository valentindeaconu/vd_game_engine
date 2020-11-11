#ifndef __PLAYER_SHADER_HPP_
#define __PLAYER_SHADER_HPP_

#include <engine/shader/Shader.hpp>

#include <engine/kernel/EngineBlock.hpp>

#include <engine/light/LightManager.hpp>

#include <engine/core/ObjectOfType.hpp>
#include <engine/misc/Properties.hpp>

namespace mod::player {
    class PlayerShader : public vd::shader::Shader {
    public:
        PlayerShader();
        ~PlayerShader();

        void updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) override;
    private:
        const size_t kMaxLights;
    };
    typedef std::shared_ptr<PlayerShader>	PlayerShaderPtr;
}

#endif // !__PLAYER_SHADER_HPP_
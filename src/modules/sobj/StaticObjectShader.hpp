#ifndef __STATIC_OBJECT_SHADER_HPP_
#define __STATIC_OBJECT_SHADER_HPP_

#include <engine/shader/Shader.hpp>

#include <engine/kernel/ObjectOfType.hpp>
#include <engine/misc/Properties.hpp>
#include <engine/kernel/Engine.hpp>
#include <engine/window/Window.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/light/LightManager.hpp>

namespace mod::sobj {
    class StaticObjectShader : public vd::shader::Shader {
    public:
        StaticObjectShader();
        ~StaticObjectShader();

        void updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) override;
    private:
        const int kMaxLights;
    };
    typedef std::shared_ptr<StaticObjectShader>	StaticObjectShaderPtr;
}

#endif // !__STATIC_OBJECT_SHADER_HPP_

//
// Created by Vali on 9/11/2020.
//

#ifndef VD_GAME_ENGINE_WATERSHADER_HPP
#define VD_GAME_ENGINE_WATERSHADER_HPP

#include <engine/shader/Shader.hpp>

#include "Water.hpp"

#include <memory>

namespace mod::water {
    class WaterShader : public vd::shader::Shader {
    public:
        WaterShader();
        ~WaterShader();

        void updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) override;
    private:
        const int kMaxLights;
    };
    typedef std::shared_ptr<WaterShader>    WaterShaderPtr;
}

#endif //VD_GAME_ENGINE_WATERSHADER_HPP

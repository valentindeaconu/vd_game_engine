//
// Created by Vali on 8/26/2020.
//

#ifndef VD_GAME_ENGINE_SHADOWSHADER_HPP
#define VD_GAME_ENGINE_SHADOWSHADER_HPP

#include <engine/component/IEntityShader.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include <engine/injector/Injectable.hpp>
#include <modules/shadow/ShadowManager.hpp>

#include <memory>

namespace mod::shadow {
    class ShadowShader : public vd::component::IEntity3DShader, public vd::injector::Injectable {
    public:
        ShadowShader();
        ~ShadowShader();

        void Link() override;

        void InitUniforms(vd::object::Entity3DPtr pEntity) override;
        void UpdateUniforms(vd::object::Entity3DPtr pEntity, uint32_t meshIndex) override;
    private:
        void AddUniforms() override;

        ShadowManagerPtr m_pShadowManager;
    };
    typedef std::shared_ptr<ShadowShader> ShadowShaderPtr;
}

#endif //VD_GAME_ENGINE_SHADOWSHADER_HPP

//
// Created by vali on 4/15/21.
//

#ifndef VDGE_MESHSHADER_HPP
#define VDGE_MESHSHADER_HPP

#include <engine/component/IEntityShader.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include <engine/injector/ObjectOfType.hpp>
#include <engine/injector/Injectable.hpp>

#include <engine/window/Window.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/light/LightManager.hpp>
#include <engine/fog/FogManager.hpp>
#include <engine/context/Context.hpp>

namespace vd {
    class MeshShader
            : public vd::component::IEntity3DShader
                    , public vd::injector::Injectable
                    , public std::enable_shared_from_this<MeshShader> {
    public:
        void Link() override;
        void Init() override;

        void InitUniforms(vd::object::Entity3DPtr pEntity) override;
        void UpdateUniforms(vd::object::Entity3DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) override;
    private:
        vd::light::LightManagerPtr  m_pLightManager;
        vd::fog::FogManagerPtr      m_pFogManager;

        vd::window::WindowPtr m_pWindow;
        vd::camera::CameraPtr m_pCamera;

        vd::context::ContextPtr m_pContext;
    };
    typedef std::shared_ptr<MeshShader>	MeshShaderPtr;
}


#endif //VDGE_MESHSHADER_HPP

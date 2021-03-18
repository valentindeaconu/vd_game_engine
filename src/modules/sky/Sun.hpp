//
// Created by Vali on 3/1/2021.
//

#ifndef VDGE_SUN_HPP
#define VDGE_SUN_HPP

#include <memory>

#include <engine/object/Entity2D.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/light/LightManager.hpp>
#include <engine/time/TimeManager.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/window/Window.hpp>
#include <engine/context/Context.hpp>

#include <engine/service/TextureService.hpp>

#include <engine/component/IRenderer.hpp>
#include <engine/loader/ShaderLoader.hpp>

namespace mod::sky {
    class Sun : public vd::object::Entity2D, public vd::injector::Injectable {
    public:
        Sun(const std::string& propsFilePath);

        void Link() override;

        void Setup() override;

        void Init() override;

        void Update() override;
    private:
        float       m_SunLightDistance;
        float       m_Radius;
        float       m_Scale;
        std::string m_TexPath;

        vd::light::LightManagerPtr  m_pLightManager;
        vd::time::TimeManagerPtr    m_pTimeManager;
        vd::camera::CameraPtr       m_pCamera;
        vd::context::ContextPtr     m_pContext;
    };
    typedef std::shared_ptr<Sun>    SunPtr;

    class SunRenderer : public vd::component::IRenderer {
    public:
        SunRenderer(SunPtr sun,
                    vd::component::IEntityShaderPtr shader,
                    vd::Consumer beforeExecution = vd::g_kEmptyConsumer,
                    vd::Consumer afterExecution = vd::g_kEmptyConsumer);

        void Init() override;
        void Update() override;
        void Render(const params_t& params) override;
        void CleanUp() override;

    private:
        bool IsReady() override;

        SunPtr m_pSun;
    };
    typedef std::shared_ptr<SunRenderer>	SunRendererPtr;

    class SunShader : public vd::component::IEntity2DShader, public vd::injector::Injectable {
    public:
        void Link() override;
        void Init() override;

        void InitUniforms(vd::object::Entity2DPtr pEntity) override;
        void UpdateUniforms(vd::object::Entity2DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) override;
    private:
        vd::camera::CameraPtr m_pCamera;
        vd::window::WindowPtr m_pWindow;
    };
    typedef std::shared_ptr<SunShader>	SunShaderPtr;
}


#endif //VDGE_SUN_HPP

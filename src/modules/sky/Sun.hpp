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
#include <engine/context/Context.hpp>

#include <engine/service/TextureService.hpp>

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

    

    
}


#endif //VDGE_SUN_HPP

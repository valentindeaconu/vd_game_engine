//
// Created by Vali on 3/1/2021.
//

#ifndef VDGE_LENSFLARERENDERER_HPP
#define VDGE_LENSFLARERENDERER_HPP

#include <memory>

#include <engine/gapi/gl/Query.hpp>
#include <engine/object/primitive/Quad2D.hpp>
#include <engine/defines/Types.hpp>
#include <engine/service/TextureService.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/window/Window.hpp>
#include <engine/context/Context.hpp>

#include <engine/component/IRenderer.hpp>
#include <engine/component/IEntityShader.hpp>

#include "Sun.hpp"
#include "LensFlareShader.hpp"

namespace mod::sky {

    class FlareRenderer : public vd::component::IRenderer, public vd::injector::Injectable {
    public:
        static const int kPriority = kDefaultPriority + 75;

        FlareRenderer(const std::string& propsFilePath);

        void Link() override;

        void OnInit() override;
        void OnUpdate() override;
        void OnRender(const params_t& params) override;
        void OnCleanUp() override;

    private:
        bool Precondition(const params_t& params) override;
        void Prepare() override;
        void Finish() override;

        size_t  m_TextureCount;
        float   m_Spacing;

        bool    m_Visible;
        float   m_Brightness;

        float   m_InvQueryQuadTotalSamples;

        glm::vec4                           m_QueryQuadPositionScale;
        std::vector<glm::vec4>              m_TexturesPositionScale;
        std::vector<vd::gl::Texture2DPtr>   m_Textures;

        vd::gl::QueryPtr                    m_pQuery;
        vd::object::primitive::Quad2DPtr    m_pQuad;
        FlareShaderPtr                      m_pShader;

        SunPtr                  m_pSun;
        vd::camera::CameraPtr   m_pCamera;
        vd::window::WindowPtr   m_pWindow;
        vd::context::ContextPtr m_pContext;
    };
    typedef std::shared_ptr<FlareRenderer>	FlareRendererPtr;

}

#endif //VDGE_LENSFLARE_HPP
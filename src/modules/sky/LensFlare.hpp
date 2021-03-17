//
// Created by Vali on 3/1/2021.
//

#ifndef VDGE_LENSFLARE_HPP
#define VDGE_LENSFLARE_HPP

#include <memory>

#include <engine/api/gl/Query.hpp>

#include <engine/object/primitive/Quad2D.hpp>

#include <engine/misc/Types.hpp>

#include <engine/service/TextureService.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/window/Window.hpp>
#include <engine/context/Context.hpp>

#include <engine/component/IRenderer.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include "Sun.hpp"

namespace mod::sky {

    class FlareRenderer : public vd::component::IRenderer, public vd::injector::Injectable {
    public:
        static const int kPriority = kDefaultPriority + 75;

        FlareRenderer(const std::string& propsFilePath,
                        vd::component::IEntityShaderPtr shader,
                        vd::Consumer beforeExecution = vd::g_kEmptyConsumer,
                        vd::Consumer afterExecution = vd::g_kEmptyConsumer);

        void Link() override;

        void Init() override;
        void Update() override;
        void Render(const params_t& params) override;
        void CleanUp() override;

    private:
        bool IsReady() override;

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

        SunPtr                  m_pSun;
        vd::camera::CameraPtr   m_pCamera;
        vd::window::WindowPtr   m_pWindow;
        vd::context::ContextPtr m_pContext;
    };
    typedef std::shared_ptr<FlareRenderer>	FlareRendererPtr;

    class FlareShader : public vd::component::IEntity2DShader, public vd::injector::Injectable {
    public:
        FlareShader();

        void Link() override;

        void InitUniforms(vd::object::Entity2DPtr pEntity) override;
        void UpdateUniforms(vd::object::Entity2DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) override;
    private:
        void AddUniforms() override;

        vd::camera::CameraPtr m_pCamera;
        vd::window::WindowPtr m_pWindow;
    };
    typedef std::shared_ptr<FlareShader>	FlareShaderPtr;
}

#endif //VDGE_LENSFLARE_HPP
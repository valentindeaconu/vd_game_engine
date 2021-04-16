//
// Created by Vali on 3/1/2021.
//

#include "LensFlareRenderer.hpp"

namespace mod::sky {

    FlareRenderer::FlareRenderer(const std::string& propsFilePath) 
        : vd::component::IRenderer("FlareRenderer")
        , m_Visible(false)
        , m_TextureCount(0)
    {
        auto pProps = vd::loader::PropertiesLoader::Load(propsFilePath);

        try {
            m_Spacing = pProps->Get<float>("Spacing");
        } catch(std::invalid_argument& e) {
            throw vd::RuntimeError("Flare spacing is not specified in the property file");
        }

        for (int i = 0; ; ++i) {
            const std::string prefix = "Texture." + std::to_string(i);
            try {
                auto path = pProps->Get<std::string>(prefix + ".Path");
                auto scale = pProps->Get<float>(prefix + ".Scale");

                m_Textures.emplace_back(vd::service::TextureService::CreateFromFile(path));
                m_TexturesPositionScale.emplace_back(glm::vec4(0.0f, 0.0f, scale, scale));

                m_TextureCount++;
            } catch (std::invalid_argument& e) {
                break;
            }
        }

        m_pQuad = std::make_shared<vd::object::primitive::Quad2D>();
        m_pQuery = std::make_shared<vd::gl::Query>(vd::gl::Query::eSamplesPassed);

        m_pShader = std::make_shared<FlareShader>();
    }

    void FlareRenderer::Link() {
        m_pSun = vd::ObjectOfType<Sun>::Find();
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
        m_pContext = vd::ObjectOfType<vd::context::Context>::Find();
    }

    void FlareRenderer::OnInit() {
        m_pQuad->Init();

        m_pShader->Init();

        m_pQuery->Create();

        for (auto& pTex : m_Textures) {
            pTex->Bind();
            pTex->LinearFilter();
            pTex->Unbind();
        }

        for (auto& posScale : m_TexturesPositionScale) {
            posScale.w *= m_pWindow->AspectRatio();
        }

        // TODO: Compute quad width based on sun's scale
        float quadWidth = 0.07f;
        m_QueryQuadPositionScale = glm::vec4(.0f, .0f, quadWidth, quadWidth * m_pWindow->AspectRatio());

        m_InvQueryQuadTotalSamples = 1.0f / (glm::pow(quadWidth * m_pWindow->Width(), 2) * m_pContext->SamplesPerPixel());
    }

    void FlareRenderer::OnUpdate() {
        glm::vec3 sunPosition = m_pSun->WorldTransform().Translation();
        glm::mat4 viewMat = m_pCamera->ViewMatrix();
        glm::mat4 projMat = m_pWindow->ProjectionMatrix();

        glm::vec4 coords = projMat * viewMat * glm::vec4(sunPosition, 1.0f);

        if (coords.w <= 0) {
            m_Visible = false;
            return;
        }

        glm::vec2 screenCoords((coords.x / coords.w + 1) / 2.0f, 1.0f - ((coords.y / coords.w + 1) / 2.0f));

        glm::vec2 toCenter = glm::vec2(.5f, .5f) - screenCoords;
        m_Brightness = 1.0f - (glm::length(toCenter) / .6f);
        if (m_Brightness <= .0f) {
            m_Visible = false;
            return;
        }

        m_Visible = true;
        m_QueryQuadPositionScale.x = screenCoords.x;
        m_QueryQuadPositionScale.y = screenCoords.y;

        for (size_t i = 0; i < m_TextureCount; ++i) {
            glm::vec2 direction =  toCenter * (i * m_Spacing);
            glm::vec2 flarePosition = screenCoords + direction;

            m_TexturesPositionScale[i].x = flarePosition.x;
            m_TexturesPositionScale[i].y = flarePosition.y;
        }
    }

    void FlareRenderer::OnRender(const params_t& params) {
        // Test Occlusion Querry
        if (m_pQuery->ResultReady()) {
            int samples = m_pQuery->GetResult();
            m_Brightness *= glm::min(samples * m_InvQueryQuadTotalSamples, 1.0f);
        }

        // Run Occlusion Querry
        if (!m_pQuery->InUse()) {
            m_pQuery->Prepare();
            m_pQuery->Start();

            m_pShader->Bind();
            m_pShader->UpdateUniforms(m_Textures[0], m_QueryQuadPositionScale, 1.0f);

            m_pQuad->Meshes()[0]->Draw();
            // m_pQuad->Buffers()[0]->DrawArrays(vd::gl::eTriangleStrip, 4);

            m_pShader->Unbind();

            m_pQuery->End();
            m_pQuery->Finish();
        }

        for (size_t i = 0; i < m_TextureCount; ++i) {
            m_pShader->Bind();
            m_pShader->UpdateUniforms(m_Textures[i], m_TexturesPositionScale[i], m_Brightness);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            // m_pQuad->Buffers()[0]->DrawArrays(vd::gl::eTriangleStrip, 4);
            m_pQuad->Meshes()[0]->Draw();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            m_pShader->Unbind();
        }
    }

    void FlareRenderer::OnCleanUp() {
        m_pQuad->CleanUp();
        m_pQuery->CleanUp();
        m_pShader->CleanUp();
        m_Textures.clear();
    }

    bool FlareRenderer::Precondition(const params_t& params) {
        if (m_pQuad == nullptr || m_pShader == nullptr || m_Textures.empty()) {
            return false;
        }

        if (!m_Visible) {
            return false;
        }

        return params.at("RenderingPass") == "Main";
    }

    void FlareRenderer::Prepare() {
        vd::gl::Context::CounterClockwiseFacing();
        vd::gl::Context::AdditiveBlending();
        vd::gl::Context::NoDepthTesting();
        vd::gl::Context::NoCulling();
    }   

    void FlareRenderer::Finish() {
        vd::gl::Context::Reset();
    } 

}
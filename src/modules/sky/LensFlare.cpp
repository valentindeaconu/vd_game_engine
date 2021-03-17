//
// Created by Vali on 3/1/2021.
//

#include "LensFlare.hpp"

namespace mod::sky {

    FlareRenderer::FlareRenderer(const std::string& propsFilePath, vd::component::IEntityShaderPtr shader,
                                 vd::Consumer beforeExecution, vd::Consumer afterExecution) 
        : vd::component::IRenderer(std::move(shader), std::move(beforeExecution), std::move(afterExecution))
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
                std::string path = pProps->Get<std::string>(prefix + ".Path");
                float scale = pProps->Get<float>(prefix + ".Scale");

                m_Textures.emplace_back(vd::service::TextureService::CreateFromFile(path));
                m_TexturesPositionScale.emplace_back(glm::vec4(0.0f, 0.0f, scale, scale));

                m_TextureCount++;
            } catch (std::invalid_argument& e) {
                break;
            }
        }

        m_pQuad = std::make_shared<vd::object::primitive::Quad2D>();
        m_pQuery = std::make_shared<vd::gl::Query>(vd::gl::Query::eSamplesPassed);
    }

    void FlareRenderer::Link() {
        m_pSun = vd::ObjectOfType<Sun>::Find();
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
        m_pContext = vd::ObjectOfType<vd::context::Context>::Find();
    }

    void FlareRenderer::Init() {
        m_pQuad->Init();
        m_pQuad->Meshes()[0]->Materials().emplace_back();

        m_pShader->Bind();
        m_pShader->InitUniforms(m_pQuad);
        m_pShader->Unbind();

        m_pQuery->Create();

        for (auto& pTex : m_Textures) {
            pTex->Bind();
            pTex->BilinearFilter();
            pTex->Unbind();
        }

        for (auto& posScale : m_TexturesPositionScale) {
            posScale.w *= m_pWindow->AspectRatio();
        }

        // TODO: Compute quad width based on sun's scale
        float quadWidth = 0.07f;
        m_QueryQuadPositionScale = glm::vec4(.0f, .0f, quadWidth, quadWidth * m_pWindow->AspectRatio());

        m_InvQueryQuadTotalSamples = 1.0f / (glm::pow(quadWidth * m_pWindow->Width() * 0.5f, 2) * m_pContext->SamplesPerPixel());
    }

    void FlareRenderer::Update() {
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

    void FlareRenderer::Render(const params_t& params) {
        if (!IsReady()) {
            vd::Logger::warn("FlareRenderer was not ready to render");
            return;
        }

        if (m_Visible) {
            auto& material = m_pQuad->Meshes()[0]->Materials()[0];

            if (params.at("RenderingPass") == "Main") {
                material.DiffuseMap() = m_Textures[0];

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
                    m_pShader->UpdateUniforms(m_pQuad, 0, 0);
                    m_pShader->SetUniform("transform", m_QueryQuadPositionScale); 
                    m_pShader->SetUniform("brightness", 1.0f);

                    m_pQuad->Buffers()[0]->DrawArrays(vd::gl::eTriangleStrip, 4);

                    m_pShader->Unbind();

                    m_pQuery->End();
                    m_pQuery->Finish();
                }

                // Render Flares
                Prepare();

                for (size_t i = 0; i < m_TextureCount; ++i) {
                    material.DiffuseMap() = m_Textures[i];

                    m_pShader->Bind();
                    m_pShader->UpdateUniforms(m_pQuad, 0, 0);
                    m_pShader->SetUniform("transform", m_TexturesPositionScale[i]); 
                    m_pShader->SetUniform("brightness", m_Brightness);

                    m_pQuad->Buffers()[0]->DrawArrays(vd::gl::eTriangleStrip, 4);

                    m_pShader->Unbind();
                }

                Finish();
            }
        }
    }

    void FlareRenderer::CleanUp() {
        m_pQuad->CleanUp();
        m_pQuery->CleanUp();
        m_Textures.clear();
    }

    bool FlareRenderer::IsReady() {
        return IRenderer::IsReady() && m_pQuad != nullptr && !m_Textures.empty();
    }

    FlareShader::FlareShader()
        : vd::component::IEntity2DShader()
    {
        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/sky/flare_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/sky/flare_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
    }

    void FlareShader::Link() {
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
    }
    
    void FlareShader::AddUniforms() {
        AddUniform("transform");
        AddUniform("brightness");

        AddUniform("diffuseMap");
    }

    void FlareShader::InitUniforms(vd::object::Entity2DPtr pEntity) {
        AddUniforms();        
    }

    void FlareShader::UpdateUniforms(vd::object::Entity2DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) {
        // SetUniform("transform", pEntity->WorldTransform().Get());

        const vd::model::Mesh2DPtr& pMesh = pEntity->Meshes()[meshIndex];
        auto& diffuseMap = pMesh->Materials()[0].DiffuseMap();

        diffuseMap->BindToUnit(0);
        SetUniform("diffuseMap", 0);
    }

}
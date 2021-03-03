//
// Created by Vali on 3/1/2021.
//

#include "Sun.hpp"

namespace mod::sky {
    Sun::Sun()
        : m_Radius(1000.0f) // TODO: This value should be computed based on terrain size
        , m_SunLightDistance(9000.0f) // TODO: This value should be read from a properties file
    {
    }

    void Sun::Link() {
        m_pLightManager = vd::ObjectOfType<vd::light::LightManager>::Find();
        m_pTimeManager = vd::ObjectOfType<vd::time::TimeManager>::Find();
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pContext = vd::ObjectOfType<vd::context::Context>::Find();
    }

    void Sun::Setup() { }

    void Sun::Init() {
        // In our scene, we assume that the earth is a perfect sphere, so the sun have to follow the same principles
        // as the clock, but a day is equivalent with 2 rotations of the clock, so we have to divide by 2 clock's angle
        // Furthermore, dividing by 2 will give us a degree interval of [0, 180], so we need to know which part of the
        // day is to simply add the other half of the circle
        // With that being said, the formula to compute sun's angle will be the following
        float angle = glm::radians(m_pTimeManager->CurrentAngle() * 0.5f + (m_pTimeManager->AM() ? 180.0f : 0));
        WorldTransform().Translation() = glm::vec3(glm::sin(angle), glm::cos(angle), 0.0f) * m_Radius;

        vd::model::Mesh2DPtr pMesh = std::make_shared<vd::model::Mesh2D>();

        pMesh->Vertices() = { vd::model::Vertex2D(.5f, .5f) };

        auto& material = pMesh->Materials().emplace_back();
        material.DiffuseMap() = vd::service::TextureService::CreateFromFile("./resources/sun/sun.png");
        material.DiffuseMap()->Bind();
        material.DiffuseMap()->NoFilter();
        material.DiffuseMap()->Unbind();

        vd::gl::BufferPtr pBuffer = std::make_shared<vd::gl::Buffer>();
        pBuffer->Bind();
        pBuffer->AddBuffer(
                vd::gl::buffer::eArrayBuffer,
                pMesh->Vertices().size() * sizeof(vd::model::Vertex3D),
                &pMesh->Vertices()[0],
                vd::gl::buffer::eStaticDraw
        );
        pBuffer->AttributeArray(0, 2, vd::gl::eFloat, sizeof(vd::model::Vertex2D), (GLvoid*)0);
        pBuffer->Unbind();

        Meshes().emplace_back(std::move(pMesh));
        Buffers().emplace_back(std::move(pBuffer));
    }

    void Sun::Update() {
        float angle = glm::radians(m_pTimeManager->CurrentAngle() * 0.5f + (m_pTimeManager->AM() ? 180.0f : 0));

        glm::vec3 sunPosition(glm::sin(angle), glm::cos(angle), 0.0f);
        sunPosition *= m_Radius;

        WorldTransform().Translation() = sunPosition;

        glm::vec3 toSun = glm::normalize(-sunPosition);
        glm::vec3 sunLightPosition = toSun * m_SunLightDistance;

        m_pLightManager->Sun()->Position() = sunLightPosition;
    }

    SunRenderer::SunRenderer(SunPtr sun,
                             vd::component::IEntityShaderPtr shader,
                             vd::Consumer beforeExecution,
                             vd::Consumer afterExecution)
        : IRenderer(std::move(shader), std::move(beforeExecution), std::move(afterExecution))
        , m_pSun(std::move(sun))
    {
    }

    void SunRenderer::Init() {
        m_pSun->Init();

        m_pShader->Bind();
        m_pShader->InitUniforms(m_pSun);
    }

    void SunRenderer::Update() {
        m_pSun->Update();
    }

    void SunRenderer::Render(const vd::datastruct::Observer::params_t &params) {
        if (!IsReady()) {
            vd::Logger::warn("SunRenderer was not ready to render");
            return;
        }

        const auto& renderingPass = params.at("RenderingPass");
        if (renderingPass == "Main") {
            Prepare();

            m_pShader->Bind();

            vd::gl::BufferPtrVec& buffers = m_pSun->Buffers();

            auto& meshes = m_pSun->Meshes();

            m_pShader->UpdateUniforms(m_pSun, 0, 0);
            buffers[0]->DrawArrays(vd::gl::ePoints, 1);

            Finish();
        }
    }

    void SunRenderer::CleanUp() {
        m_pSun->CleanUp();
        m_pSun = nullptr;
    }

    bool SunRenderer::IsReady() {
        return IRenderer::IsReady() && m_pSun != nullptr;
    }

    SunShader::SunShader() : vd::component::IEntity2DShader() {
        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/sun/sun_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string gSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/sun/sun_GS.glsl", gSource);
        AddShader(gSource, vd::gl::Shader::eGeometryShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/sun/sun_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
    }

    void SunShader::Link() {
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pWindow = vd::ObjectOfType<vd::window::Window>::Find();
    }

    void SunShader::AddUniforms() {
        AddUniform("model");
        AddUniform("view");
        AddUniform("projection");

        AddUniform("diffuseMap");

        AddUniform("cameraUp");
        AddUniform("cameraRight");

    }

    void SunShader::InitUniforms(vd::object::Entity2DPtr pEntity) {
        AddUniforms();
    }

    void SunShader::UpdateUniforms(vd::object::Entity2DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) {
        SetUniform("model", pEntity->WorldTransform().Get());
        SetUniform("view", m_pCamera->ViewMatrix());
        SetUniform("projection", m_pWindow->ProjectionMatrix());

        SetUniform("cameraUp", m_pCamera->Up());
        SetUniform("cameraRight", m_pCamera->Right());

        const vd::model::Mesh2DPtr& pMesh = pEntity->Meshes()[meshIndex];
        auto& diffuseMap = pMesh->Materials()[0].DiffuseMap();

        vd::gl::ActiveTexture(0);
        diffuseMap->Bind();
        SetUniform("diffuseMap", 0);
    }
}
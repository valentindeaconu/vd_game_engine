//
// Created by Vali on 3/1/2021.
//

#include "Sun.hpp"

namespace mod::sky {
    
    Sun::Sun(const std::string& propsFilePath) {
        auto pProps = vd::loader::PropertiesLoader::Load(propsFilePath);

        try {
            m_Radius = pProps->Get<float>("Radius");
            m_SunLightDistance = pProps->Get<float>("LightDistance");
            m_Scale = pProps->Get<float>("Scale");
            m_TexPath = pProps->Get<std::string>("Texture");
        } catch(std::invalid_argument& e) {
            throw vd::RuntimeError("Sun property file does not provide radius, light distance or scale");
        }
    }

    void Sun::Link() {
        m_pLightManager = vd::ObjectOfType<vd::light::LightManager>::Find();
        m_pTimeManager = vd::ObjectOfType<vd::time::TimeManager>::Find();
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
        m_pContext = vd::ObjectOfType<vd::context::Context>::Find();
    }

    void Sun::Setup() { }

    void Sun::Init() {
        // Set scale to entity's local transform
        LocalTransform().Scale() = glm::vec3(m_Scale);

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
        material.DiffuseMap() = vd::service::TextureService::CreateFromFile(m_TexPath);
        material.DiffuseMap()->Bind();
        material.DiffuseMap()->NoFilter();
        material.DiffuseMap()->Unbind();

        vd::gl::BufferPtr pBuffer = std::make_shared<vd::gl::Buffer>();
        pBuffer->Create();
        pBuffer->Bind();
        pBuffer->AddBuffer(
                vd::gl::eArrayBuffer,
                pMesh->Vertices().size() * sizeof(vd::model::Vertex2D),
                &pMesh->Vertices()[0],
                vd::gl::eStaticDraw
        );
        pBuffer->AttributeArray(0, 0, 2, vd::gl::eFloat, sizeof(vd::model::Vertex2D), (GLvoid*)0);
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

}
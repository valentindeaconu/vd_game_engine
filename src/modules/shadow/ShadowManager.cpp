//
// Created by Vali on 8/28/2020.
//

#include "ShadowManager.hpp"

namespace mod::shadow {
    ShadowManager::ShadowManager(const std::string& propsFilePath)
        : m_pFrameBuffer(nullptr)
    {
        auto pProperties = vd::loader::PropertiesLoader::Load(propsFilePath);

        m_MapSize = pProperties->Get<int>("Shadow.MapSize");
        m_Distance = pProperties->Get<int>("Shadow.Distance");
        m_TransitionDistance = pProperties->Get<int>("Shadow.TransitionDistance");
        m_Offset = pProperties->Get<float>("Shadow.Offset");

        m_pView = std::make_shared<glm::mat4>(1.0f);
        m_pProjection = std::make_shared<glm::mat4>(1.0f);
        m_pFrameBuffer = std::make_shared<vd::gl::FrameBuffer>();
    }

    ShadowManager::~ShadowManager() = default;

    void ShadowManager::Link() {
        auto& lightManagerPtr = vd::ObjectOfType<vd::light::LightManager>::Find();
        m_pSun = lightManagerPtr->Sun();
    }

    void ShadowManager::Init() {
        m_pFrameBuffer->Bind();
        m_pFrameBuffer->Resize(m_MapSize, m_MapSize);

        m_pFrameBuffer->PushAttachment(vd::gl::FrameBuffer::eDepthTexture, [](vd::gl::Texture2DPtr& pTex) {
            pTex->Bind();

            pTex->BilinearFilter();

            pTex->WrapClampToBorder();
            float border[] = {1.0f, 1.0f, 1.0f, 1.0f};

            pTex->Parameter(vd::gl::TextureParameter::eTextureBorderColor, border);

            pTex->Unbind();
        });

        if (m_pFrameBuffer->Status() != vd::gl::FrameBuffer::eComplete) {
            throw vd::RuntimeError("framebuffer is incomplete or has errors");
        }

        m_pFrameBuffer->Unbind();

        m_pShadowBox = std::make_shared<ShadowBox>(m_pView, m_Distance, m_Offset);

        m_pShadowBox->Link();
    }

    void ShadowManager::Update() {
        m_pShadowBox->Update();

        UpdateView();
        UpdateProjection();
    }

    void ShadowManager::CleanUp() {
        m_pFrameBuffer = nullptr;
    }

    float ShadowManager::Distance() const {
        return m_Distance;
    }

    float ShadowManager::TransitionDistance() const {
        return m_TransitionDistance;
    }

    const vd::gl::FrameBufferPtr &ShadowManager::FrameBuffer() const {
        return m_pFrameBuffer;
    }

    const vd::gl::Texture2DPtr& ShadowManager::ShadowTexture() const {
        return m_pFrameBuffer->DepthTexture();
    }

    const glm::mat4& ShadowManager::ViewMatrix() const {
        return *m_pView;
    }

    const glm::mat4& ShadowManager::ProjectionMatrix() const {
        return *m_pProjection;
    }

    void ShadowManager::UpdateView() {
        const glm::vec3 center = -m_pShadowBox->Center();
        const glm::vec3 lightDirection = glm::normalize(-m_pSun->Direction());

        const glm::vec3 x_unit = glm::vec3(1.0f, 0.0f, 0.0f);
        const glm::vec3 y_unit = glm::vec3(0.0f, 1.0f, 0.0f);

        float pitch = (float) std::acos(glm::length(glm::vec2(lightDirection.x, lightDirection.z)));

        float yaw = glm::degrees((float) std::atan2(lightDirection.x, lightDirection.z));
        yaw = lightDirection.z > 0 ? yaw - 180 : yaw;
        yaw = (float) glm::radians(yaw);

        *m_pView = glm::rotate(glm::mat4(1.0f), pitch, x_unit);
        *m_pView = glm::rotate(*m_pView, yaw, y_unit);
        *m_pView = glm::translate(*m_pView, center);
    }

    void ShadowManager::UpdateProjection() {
        *m_pProjection = glm::mat4(1.0f);
        (*m_pProjection)[0][0] = 2.0f / (m_pShadowBox->X().max - m_pShadowBox->X().min);
        (*m_pProjection)[1][1] = 2.0f / (m_pShadowBox->Y().max - m_pShadowBox->Y().min);
        (*m_pProjection)[2][2] = -2.0f / (m_pShadowBox->Z().max - m_pShadowBox->Z().min);
        (*m_pProjection)[3][3] = 1.0f;
    }

}
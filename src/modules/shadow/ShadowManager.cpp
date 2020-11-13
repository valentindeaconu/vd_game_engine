//
// Created by Vali on 8/28/2020.
//

#include "ShadowManager.hpp"

namespace mod::shadow {
    ShadowManager::ShadowManager()
        : m_MapSize(0)
        , m_Distance(0.0f)
        , m_TransitionDistance(0.0f)
        , m_FrameBufferPtr(nullptr)
    {
        m_ViewPtr = std::make_shared<glm::mat4>(1.0f);
        m_ProjectionPtr = std::make_shared<glm::mat4>(1.0f);
        m_FrameBufferPtr = std::make_shared<vd::gl::FrameBuffer>();
    }

    ShadowManager::~ShadowManager() = default;

    void ShadowManager::Init() {
        auto& propertiesPtr = vd::ObjectOfType<vd::misc::Properties>::Find();

        m_MapSize = propertiesPtr->Get<int>("Shadow.MapSize");
        m_Distance = propertiesPtr->Get<int>("Shadow.Distance");
        m_TransitionDistance = propertiesPtr->Get<int>("Shadow.TransitionDistance");

        m_FrameBufferPtr->Allocate(m_MapSize, m_MapSize, false, vd::gl::DepthAttachment::eDepthTexture);

        m_FrameBufferPtr->GetDepthTexture()->Bind();

        m_FrameBufferPtr->GetDepthTexture()->NoFilter();
        m_FrameBufferPtr->GetDepthTexture()->WrapClampToBorder();

        float border[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

        m_FrameBufferPtr->GetDepthTexture()->Unbind();

        m_FrameBufferPtr->Bind();

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        m_FrameBufferPtr->Unbind();

        m_ShadowBoxPtr = std::make_shared<ShadowBox>(m_ViewPtr, m_Distance, propertiesPtr->Get<float>("Shadow.Offset"));

        m_ShadowBoxPtr->Link();

        auto& lightManagerPtr = vd::ObjectOfType<vd::light::LightManager>::Find();
        m_SunPtr = lightManagerPtr->GetSun();
    }

    void ShadowManager::Update() {
        m_ShadowBoxPtr->Update();

        UpdateView();
        UpdateProjection();
    }

    void ShadowManager::CleanUp() {
        m_FrameBufferPtr->CleanUp();
    }

    float ShadowManager::Distance() const {
        return m_Distance;
    }

    float ShadowManager::TransitionDistance() const {
        return m_TransitionDistance;
    }

    const vd::gl::FrameBufferPtr &ShadowManager::FrameBuffer() const {
        return m_FrameBufferPtr;
    }

    const vd::gl::Texture2DPtr& ShadowManager::ShadowTexture() const {
        return m_FrameBufferPtr->GetDepthTexture();
    }

    const glm::mat4& ShadowManager::ViewMatrix() const {
        return *m_ViewPtr;
    }

    const glm::mat4& ShadowManager::ProjectionMatrix() const {
        return *m_ProjectionPtr;
    }

    void ShadowManager::UpdateView() {
        const glm::vec3 center = -m_ShadowBoxPtr->Center();
        const glm::vec3 lightDirection = glm::normalize(-m_SunPtr->GetDirection());

        const glm::vec3 x_unit = glm::vec3(1.0f, 0.0f, 0.0f);
        const glm::vec3 y_unit = glm::vec3(0.0f, 1.0f, 0.0f);

        float pitch = (float) std::acos(glm::length(glm::vec2(lightDirection.x, lightDirection.z)));

        float yaw = glm::degrees((float) std::atan2(lightDirection.x, lightDirection.z));
        yaw = lightDirection.z > 0 ? yaw - 180 : yaw;
        yaw = (float) glm::radians(yaw);

        *m_ViewPtr = glm::rotate(glm::mat4(1.0f), pitch, x_unit);
        *m_ViewPtr = glm::rotate(*m_ViewPtr, yaw, y_unit);
        *m_ViewPtr = glm::translate(*m_ViewPtr, center);
    }

    void ShadowManager::UpdateProjection() {
        *m_ProjectionPtr = glm::mat4(1.0f);
        (*m_ProjectionPtr)[0][0] = 2.0f / (m_ShadowBoxPtr->X().max - m_ShadowBoxPtr->X().min);
        (*m_ProjectionPtr)[1][1] = 2.0f / (m_ShadowBoxPtr->Y().max - m_ShadowBoxPtr->Y().min);
        (*m_ProjectionPtr)[2][2] = -2.0f / (m_ShadowBoxPtr->Z().max - m_ShadowBoxPtr->Z().min);
        (*m_ProjectionPtr)[3][3] = 1.0f;
    }

}
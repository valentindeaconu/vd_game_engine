//
// Created by Vali on 8/28/2020.
//

#include "ShadowManager.hpp"

namespace vd::shadow {
    ShadowManager::ShadowManager()
        : mapSize(0)
        , distance(0.0f)
        , transitionDistance(0.0f)
        , frameBufferPtr(nullptr)
    {
        lightViewPtr = std::make_shared<glm::mat4>(1.0f);
        projectionPtr = std::make_shared<glm::mat4>(1.0f);
        frameBufferPtr = std::make_shared<buffer::FrameBuffer>();
    }

    ShadowManager::~ShadowManager() = default;

    void ShadowManager::init(const core::WindowPtr& windowPtr,
                             const core::CameraPtr& cameraPtr,
                             int mapSize,
                             float distance,
                             float transitionDistance,
                             float offset)
    {
        this->mapSize = mapSize;
        this->distance = distance;
        this->transitionDistance = transitionDistance;

        this->frameBufferPtr->Allocate(mapSize,
                                       mapSize,
                                       false,
                                       buffer::DepthAttachment::eDepthTexture);

        this->frameBufferPtr->GetDepthTexture()->bind();

        this->frameBufferPtr->GetDepthTexture()->noFilter();
        this->frameBufferPtr->GetDepthTexture()->wrapClampToBorder();

        float border[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

        this->frameBufferPtr->GetDepthTexture()->unbind();

        this->frameBufferPtr->Bind();

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        this->frameBufferPtr->Unbind();

        shadowBoxPtr = std::make_shared<ShadowBox>(windowPtr, cameraPtr, lightViewPtr, distance, offset);
    }

    void ShadowManager::update(const light::LightPtr& sunPtr) {
        shadowBoxPtr->update();
        updateLightView(sunPtr);
        updateProjection();
    }

    void ShadowManager::cleanUp() {
        frameBufferPtr->CleanUp();
    }

    float ShadowManager::getDistance() const {
        return distance;
    }

    float ShadowManager::getTransitionDistance() const {
        return transitionDistance;
    }

    const buffer::FrameBufferPtr &ShadowManager::getFramebuffer() const {
        return frameBufferPtr;
    }

    const model::Texture2DPtr& ShadowManager::getShadowTexture() const {
        return frameBufferPtr->GetDepthTexture();
    }

    const glm::mat4& ShadowManager::getViewMatrix() const {
        return *lightViewPtr;
    }

    const glm::mat4& ShadowManager::getProjectionMatrix() const {
        return *projectionPtr;
    }

    void ShadowManager::updateLightView(const light::LightPtr& sunPtr) {
        const glm::vec3 center = -shadowBoxPtr->getCenter();
        const glm::vec3 lightDirection = glm::normalize(-sunPtr->GetDirection());

        const glm::vec3 x_unit = glm::vec3(1.0f, 0.0f, 0.0f);
        const glm::vec3 y_unit = glm::vec3(0.0f, 1.0f, 0.0f);

        float pitch = (float) std::acos(glm::length(glm::vec2(lightDirection.x, lightDirection.z)));

        float yaw = glm::degrees((float) std::atan2(lightDirection.x, lightDirection.z));
        yaw = lightDirection.z > 0 ? yaw - 180 : yaw;
        yaw = (float) glm::radians(yaw);


        *lightViewPtr = glm::rotate(glm::mat4(1.0f), pitch, x_unit);
        *lightViewPtr = glm::rotate(*lightViewPtr, yaw, y_unit);
        *lightViewPtr = glm::translate(*lightViewPtr, center);
    }

    void ShadowManager::updateProjection() {
        /**projectionPtr = glm::ortho(shadowBoxPtr->getX().min, shadowBoxPtr->getX().max,
                shadowBoxPtr->getY().min, shadowBoxPtr->getY().max,
                shadowBoxPtr->getZ().min, shadowBoxPtr->getZ().max);*/

        *projectionPtr = glm::mat4(1.0f);
        (*projectionPtr)[0][0] = 2.0f / (shadowBoxPtr->getX().max - shadowBoxPtr->getX().min);
        (*projectionPtr)[1][1] = 2.0f / (shadowBoxPtr->getY().max - shadowBoxPtr->getY().min);
        (*projectionPtr)[2][2] = -2.0f / (shadowBoxPtr->getZ().max - shadowBoxPtr->getZ().min);
        (*projectionPtr)[3][3] = 1.0f;
    }

}
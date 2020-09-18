//
// Created by Vali on 9/11/2020.
//

#include "Water.hpp"

namespace mod::water {
    Water::Water(const vd::EnginePtr &enginePtr, const std::string &configFilePath)
        : vd::object::Entity(enginePtr)
    {
        configPtr = std::make_shared<WaterConfig>(configFilePath);

        reflectionFBO = std::make_shared<vd::buffer::FrameBuffer>();
        refractionFBO = std::make_shared<vd::buffer::FrameBuffer>();
    }

    Water::~Water() = default;

    void Water::init() {
        getLocalTransform().setScaling(3072.0f, 0.0f, 3072.0f);
        getLocalTransform().setTranslation(-1024.0f, -33.0f, -1024.0f);

        // asta merge also
        //getLocalTransform().setScaling(250.0f, 0.0f, 260.0f);
        //getLocalTransform().setTranslation(400.0f, -33.0f, 300.0f);

        configPtr->parse();

        generatePatch();

        reflectionFBO->allocate(configPtr->getReflectionWidth(),
                                configPtr->getReflectionHeight(),
                                true,
                                vd::buffer::DepthAttachment::eDepthBuffer);

        refractionFBO->allocate(configPtr->getRefractionWidth(),
                                configPtr->getRefractionHeight(),
                                true,
                                vd::buffer::DepthAttachment::eDepthTexture);

        Entity::init();
    }

    void Water::update() {
        moveFactor += (configPtr->getWaveSpeed() * getParentEngine()->getFrameTime());
        if (moveFactor >= 1.0f) {
            moveFactor -= 1.0f;
        }
    }

    void Water::cleanUp() {
        reflectionFBO->cleanUp();
        refractionFBO->cleanUp();

        Entity::cleanUp();
    }

    const WaterConfigPtr &Water::getWaterConfig() const {
        return configPtr;
    }

    const vd::buffer::FrameBufferPtr &Water::getReflectionFramebuffer() const {
        return reflectionFBO;
    }

    const vd::buffer::FrameBufferPtr &Water::getRefractionFramebuffer() const {
        return refractionFBO;
    }

    float Water::getHeight() const {
        return -33.0f;
    }

    float Water::getMoveFactor() const {
       return moveFactor; //return (float) std::sin(moveFactor * 2 * std::numbers::pi) / 2.0f + 0.5f;
    }

    void Water::generatePatch() {
        vd::model::MeshPtr meshPtr = std::make_shared<vd::model::Mesh>();

        meshPtr->vertices = {
                { .Position = glm::vec3(-1.0f, 0.0f, 1.0f) },
                { .Position = glm::vec3(-1.0f, 0.0f, -1.0f) },
                { .Position = glm::vec3(1.0f, 0.0f, 1.0f) },
                { .Position = glm::vec3(1.0f, 0.0f, -1.0f) }
        };

        meshPtr->indices = { 0, 2, 1, 1, 2, 3 };

        getMeshes().push_back(meshPtr);
    }
}


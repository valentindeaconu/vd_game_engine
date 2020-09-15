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
        getLocalTransform().setScaling(768.0f, 768.0f, 768.0f);
        getLocalTransform().setTranslation(-1024.0f, -33.0f, -1024.0f);

        configPtr->parse();

        generatePatch();

        reflectionFBO->allocate(configPtr->getReflectionWidth(),
                                configPtr->getReflectionHeight(),
                                vd::buffer::DepthAttachment::eDepthBuffer);

        refractionFBO->allocate(configPtr->getRefractionWidth(),
                                configPtr->getRefractionHeight(),
                                vd::buffer::DepthAttachment::eDepthTexture);

        Entity::init();
    }

    void Water::update() {

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

    void Water::generatePatch() {
        vd::model::MeshPtr meshPtr = std::make_shared<vd::model::Mesh>();

        const size_t size = configPtr->getSize();

        for (size_t i = 0; i <= size; ++i)
        {
            for (size_t j = 0; j <= size; ++j)
            {
                meshPtr->vertices.emplace_back();
                vd::model::Vertex& v = meshPtr->vertices.back();

                float x = ((float)i / size);
                float z = ((float)j / size);

                v.Position = glm::vec3(i, 0.0f, j);
                v.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
                v.TexCoords = glm::vec2(x, z);
            }
        }

        for (size_t i = 1; i <= size; ++i)
        {
            for (size_t j = 1; j <= size; ++j)
            {
                GLuint current = i * (size + 1) + j;
                GLuint west = current - 1;
                GLuint north = (i - 1) * (size + 1) + j;
                GLuint northWest = north - 1;

                meshPtr->indices.push_back(current);
                meshPtr->indices.push_back(west);
                meshPtr->indices.push_back(northWest);

                meshPtr->indices.push_back(current);
                meshPtr->indices.push_back(northWest);
                meshPtr->indices.push_back(north);
            }
        }

        getMeshes().push_back(meshPtr);
    }
}


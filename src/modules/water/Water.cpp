//
// Created by Vali on 9/11/2020.
//

#include "Water.hpp"

namespace mod::water {
    Water::Water(const std::string& propsFilePath)
        : m_PropsPtr(vd::misc::Properties::Create<vd::misc::Properties::eFile>(propsFilePath))
        , m_MoveFactor(0.0f)
        , m_ReflectionFBO(std::make_shared<vd::buffer::FrameBuffer>())
        , m_RefractionFBO(std::make_shared<vd::buffer::FrameBuffer>())
    {
    }

    Water::~Water() = default;

    void Water::Init() {
        m_EnginePtr = vd::ObjectOfType<vd::Engine>::Find();

        GetLocalTransform().setScaling(6000.0f, 0.0f, 6000.0f);
        GetLocalTransform().setTranslation(-3000.0f, 180.0f, -3000.0f);

        PopulatePacks();

        GeneratePatch();

        m_ReflectionFBO->Allocate(m_PropsPtr->Get<int>("Reflection.Width"),
                                  m_PropsPtr->Get<int>("Reflection.Height"),
                                  true,
                                  vd::buffer::DepthAttachment::eDepthBuffer);

        m_RefractionFBO->Allocate(m_PropsPtr->Get<int>("Refraction.Width"),
                                  m_PropsPtr->Get<int>("Refraction.Height"),
                                  true,
                                  vd::buffer::DepthAttachment::eDepthTexture);

        Entity::Init();
    }

    void Water::Update() {
        const auto waveSpeed = m_PropsPtr->Get<float>("Wave.Speed");
        m_MoveFactor += (waveSpeed * m_EnginePtr->getFrameTime());
        if (m_MoveFactor >= 1.0f) {
            m_MoveFactor -= 1.0f;
        }
    }

    void Water::CleanUp() {
        m_ReflectionFBO->CleanUp();
        m_RefractionFBO->CleanUp();

        Entity::CleanUp();
    }

    const vd::misc::PropertiesPtr& Water::GetProperties() const {
        return m_PropsPtr;
    }

    const vd::model::Material& Water::GetMaterial() const {
        auto it = m_PacksMap.find(m_CurrentPack);
        if (it != m_PacksMap.end()) {
            return it->second;
        }

        throw std::runtime_error("water current pack '" + m_CurrentPack + "' does not exists");
    }

    const vd::buffer::FrameBufferPtr &Water::GetReflectionFramebuffer() const {
        return m_ReflectionFBO;
    }

    const vd::buffer::FrameBufferPtr &Water::GetRefractionFramebuffer() const {
        return m_RefractionFBO;
    }

    float Water::GetHeight() const {
        return 180.0f;
    }

    float Water::GetMoveFactor() const {
        //return (float) std::sin(moveFactor * 2 * std::numbers::pi) / 2.0f + 0.5f;
        return m_MoveFactor;
    }

    void Water::PopulatePacks() {
        for (int i = 0; ; ++i) {
            try {
                const std::string prefix = "Pack." + std::to_string(i);

                vd::model::Material material;

                material.name = m_PropsPtr->Get<std::string>(prefix + ".Name");
                material.displaceMap = vd::model::TextureService::get(m_PropsPtr->Get<std::string>(prefix + ".DuDv"));
                material.displaceMap->bind();
                material.displaceMap->bilinearFilter();
                material.displaceMap->unbind();

                material.normalMap = vd::model::TextureService::get(m_PropsPtr->Get<std::string>(prefix + ".Normal"));
                material.normalMap->bind();
                material.normalMap->bilinearFilter();
                material.normalMap->unbind();

                m_PacksMap[material.name] = material;

                if (i == 0) {
                    m_CurrentPack = material.name;
                }
            } catch (std::invalid_argument& e) {
                if (i == 0) {
                    throw std::logic_error("water should have at least 1 material");
                }
                break;
            }
        }
    }

    void Water::GeneratePatch() {
        vd::model::MeshPtr meshPtr = std::make_shared<vd::model::Mesh>();

        meshPtr->vertices = {
                { .Position = glm::vec3(0.0f, 0.0f, 1.0f) },
                { .Position = glm::vec3(0.0f, 0.0f, 0.0f) },
                { .Position = glm::vec3(1.0f, 0.0f, 1.0f) },
                { .Position = glm::vec3(1.0f, 0.0f, 0.0f) }
        };

        meshPtr->indices = { 0, 2, 1, 1, 2, 3 };

        GetMeshes().push_back(meshPtr);
    }
}


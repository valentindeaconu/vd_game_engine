//
// Created by Vali on 9/11/2020.
//

#include "Water.hpp"

namespace mod::water {
    Water::Water(const std::string& propsFilePath)
        : m_pProperties(vd::loader::PropertiesLoader::Load(propsFilePath))
        , m_MoveFactor(0.0f)
        , m_pReflectionFBO(std::make_shared<vd::gl::FrameBuffer>())
        , m_pRefractionFBO(std::make_shared<vd::gl::FrameBuffer>())
    {
    }

    Water::~Water() = default;

    void Water::Link() {
        m_pContext = vd::ObjectOfType<vd::kernel::Context>::Find();
    }

    void Water::Setup() {
        LocalTransform().Scale() = glm::vec3(6000.0f, 0.0f, 6000.0f);
        LocalTransform().Translation() = glm::vec3(-3000.0f, 180.0f, -3000.0f);

        PopulatePacks();

        GeneratePatch();

        m_pReflectionFBO->Allocate(m_pProperties->Get<int>("Reflection.Width"),
                                   m_pProperties->Get<int>("Reflection.Height"),
                                   true,
                                   vd::gl::DepthAttachment::eDepthBuffer);

        m_pRefractionFBO->Allocate(m_pProperties->Get<int>("Refraction.Width"),
                                   m_pProperties->Get<int>("Refraction.Height"),
                                   true,
                                   vd::gl::DepthAttachment::eDepthTexture);
    }

    void Water::Update() {
        const auto waveSpeed = m_pProperties->Get<float>("Wave.Speed");
        m_MoveFactor += (waveSpeed * m_pContext->FrameTime());
        if (m_MoveFactor >= 1.0f) {
            m_MoveFactor -= 1.0f;
        }
    }

    void Water::CleanUp() {
        m_pReflectionFBO->CleanUp();
        m_pRefractionFBO->CleanUp();

        Entity3D::CleanUp();
    }

    const vd::property::PropertiesPtr& Water::Properties() const {
        return m_pProperties;
    }

    const vd::model::Material& Water::Material() const {
        auto it = m_PacksMap.find(m_CurrentPack);
        if (it != m_PacksMap.end()) {
            return it->second;
        }

        throw std::runtime_error("water current pack '" + m_CurrentPack + "' does not exists");
    }

    const vd::gl::FrameBufferPtr &Water::ReflectionFramebuffer() const {
        return m_pReflectionFBO;
    }

    const vd::gl::FrameBufferPtr &Water::RefractionFramebuffer() const {
        return m_pRefractionFBO;
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

                vd::model::Material material(m_pProperties->Get<std::string>(prefix + ".Name"));

                material.DisplaceMap() =
                        vd::service::TextureService::CreateFromFile(m_pProperties->Get<std::string>(prefix + ".DuDv"));
                material.DisplaceMap()->Bind();
                material.DisplaceMap()->BilinearFilter();
                material.DisplaceMap()->Unbind();

                material.NormalMap() =
                        vd::service::TextureService::CreateFromFile(m_pProperties->Get<std::string>(prefix + ".Normal"));
                material.NormalMap()->Bind();
                material.NormalMap()->BilinearFilter();
                material.NormalMap()->Unbind();

                m_PacksMap[material.Name()] = material;

                if (i == 0) {
                    m_CurrentPack = material.Name();
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
        vd::model::Mesh3DPtr meshPtr = std::make_shared<vd::model::Mesh3D>();

        meshPtr->Vertices() = {
            vd::model::Vertex3D(0.0f, 0.0f, 1.0f),
            vd::model::Vertex3D(0.0f, 0.0f, 0.0f),
            vd::model::Vertex3D(1.0f, 0.0f, 1.0f),
            vd::model::Vertex3D(1.0f, 0.0f, 0.0f)
        };

        meshPtr->Indices() = {0, 2, 1, 1, 2, 3};

        Meshes().push_back(meshPtr);
    }
}


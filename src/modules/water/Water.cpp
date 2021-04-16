//
// Created by Vali on 9/11/2020.
//

#include "Water.hpp"

namespace mod::water {
    Water::Water(const std::string& propsFilePath)
        : m_pProperties(vd::loader::PropertiesLoader::Load(propsFilePath))
        , m_MoveFactor(0.0f)
    {
        m_pReflectionFBO = std::make_shared<vd::gl::FrameBuffer>(m_pProperties->Get<int>("Reflection.Width"),
                                                                 m_pProperties->Get<int>("Reflection.Height"));
        m_pRefractionFBO = std::make_shared<vd::gl::FrameBuffer>(m_pProperties->Get<int>("Refraction.Width"),
                                                                 m_pProperties->Get<int>("Refraction.Height"));
    }

    Water::~Water() = default;

    void Water::Link() {
        m_pContext = vd::ObjectOfType<vd::context::Context>::Find();
    }

    void Water::Setup() {
        LocalTransform().Scale() = glm::vec3(1024.0f, 0.0f, 1024.0f);
        LocalTransform().Translation() = glm::vec3(-512.0f, 30.0f, -512.0f);

        PopulatePacks();

        GeneratePatch();

        m_pReflectionFBO->Create();
        m_pReflectionFBO->Bind();

        m_pReflectionFBO->PushAttachment(vd::gl::FrameBuffer::eColorTexture, [](vd::gl::Texture2DPtr& pTex) {
            pTex->Bind();
            pTex->LinearFilter();
            pTex->Unbind();
        });
        m_pReflectionFBO->PushAttachment(vd::gl::FrameBuffer::eDepthBuffer);
        m_pReflectionFBO->Unbind();

        m_pRefractionFBO->Create();
        m_pRefractionFBO->Bind();

        m_pRefractionFBO->PushAttachment(vd::gl::FrameBuffer::eColorTexture, [](vd::gl::Texture2DPtr& pTex) {
            pTex->Bind();
            pTex->LinearFilter();
            pTex->Unbind();
        });
        m_pRefractionFBO->PushAttachment(vd::gl::FrameBuffer::eDepthTexture, [](vd::gl::Texture2DPtr& pTex) {
            pTex->Bind();
            pTex->LinearFilter();
            pTex->Unbind();
        });
        m_pRefractionFBO->Unbind();
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
        return 30.0f;
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
                material.DisplaceMap()->LinearFilter();
                material.DisplaceMap()->Unbind();

                material.NormalMap() =
                        vd::service::TextureService::CreateFromFile(m_pProperties->Get<std::string>(prefix + ".Normal"));
                material.NormalMap()->Bind();
                material.NormalMap()->LinearFilter();
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
        vd::model::MeshPtr pMesh = std::make_shared<vd::model::Mesh>(vd::gapi::AttributeTypeVec ({ vd::gapi::AttributeType::FLOAT_3 }));

        pMesh->Assign(vd::gapi::DataFragmentation::eAsTriangles,
        {
            vd::model::Vertex(std::vector<float>{ 0.0f, 0.0f, 1.0f }),
            vd::model::Vertex(std::vector<float>{ 0.0f, 0.0f, 0.0f }),
            vd::model::Vertex(std::vector<float>{ 1.0f, 0.0f, 1.0f }),
            vd::model::Vertex(std::vector<float>{ 1.0f, 0.0f, 0.0f })
        },
        {0, 2, 1, 1, 2, 3});

        this->PushMesh({ pMesh }, 1000.0f);
    }
}


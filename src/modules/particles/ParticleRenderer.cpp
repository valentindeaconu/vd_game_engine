//
// Created by Vali on 4/3/2021.
//

#include "ParticleRenderer.hpp"

namespace mod::particles {
    ParticleRenderer::ParticleRenderer(ParticleSystemPtr particleSystem) 
        : vd::component::IRenderer("ParticleRenderer")
        , m_pSystem(std::move(particleSystem))
    {
        m_pShader = std::make_shared<ParticleShader>();
    }

    void ParticleRenderer::Link() {
        m_pEventHandler = vd::ObjectOfType<vd::event::EventHandler>::Find();
        m_pContext = vd::ObjectOfType<vd::context::Context>::Find();
        m_pPlayer = vd::ObjectOfType<mod::player::Player>::Find();
    }

    void ParticleRenderer::OnInit() {
        m_pBuffer = std::make_shared<vd::gl::Buffer>();
        m_pBuffer->Create();
        m_pBuffer->Bind();

        const uint32_t kInstanceDataLength = (4 * 4 + 1); // 4x4 mat + 1 float life percentage
        const uint32_t kMaxInstances = 10000; // TODO: Move this to a define / property file

        m_DataLength = kMaxInstances * kInstanceDataLength;

        const size_t maxDataLengthInBytes = (m_DataLength << 2); // 4 bytes per float
        const size_t instanceDataLength = (kInstanceDataLength << 2); // 4 bytes per float

        struct Data {
            glm::vec4 c0;
            glm::vec4 c1;
            glm::vec4 c2;
            glm::vec4 c3;
            float lp;
        };

        m_pBuffer->AddBuffer(vd::gl::eArrayBuffer, maxDataLengthInBytes, nullptr, vd::gl::eDynamicDraw);
        m_pBuffer->AttributeArray(0, 0, 4, vd::gl::eFloat, instanceDataLength, (GLvoid*)offsetof(Data, c0));
        m_pBuffer->AttributeArray(1, 0, 4, vd::gl::eFloat, instanceDataLength, (GLvoid*)offsetof(Data, c1));
        m_pBuffer->AttributeArray(2, 0, 4, vd::gl::eFloat, instanceDataLength, (GLvoid*)offsetof(Data, c2));
        m_pBuffer->AttributeArray(3, 0, 4, vd::gl::eFloat, instanceDataLength, (GLvoid*)offsetof(Data, c3));
        m_pBuffer->AttributeArray(4, 0, 1, vd::gl::eFloat, instanceDataLength, (GLvoid*)offsetof(Data, lp));

        m_pBuffer->Unbind();

        m_pShader->Init();
    }

    void ParticleRenderer::OnUpdate() {
        if (m_pEventHandler->KeyDown(vd::Key::eU)) {
            glm::vec3 pos = m_pPlayer->WorldTransform().Translation();

            m_Batch.emplace_back(
                std::make_shared<Particle>(pos, glm::vec3(0, 30, 0), 1, 1.50f, 0, 0.25f)
            );
        }

        ParticlePtrVec particles;
        m_pSystem->GenerateParticles(m_pPlayer->WorldTransform().Translation(), m_pContext->FrameTime(), particles);

        for (auto& particle : particles) {
            m_Batch.emplace_back(std::move(particle));
        }

        m_BufferData.clear();
        m_BufferData.reserve(m_DataLength);
        m_ParticleCount = 0;

        std::vector<std::list<ParticlePtr>::iterator> garbageCollector;
        for (auto it = m_Batch.begin(); it != m_Batch.end(); ++it) {
            (*it)->Update(m_pContext->FrameTime());
            if ((*it)->Alive()) {
                glm::mat4 model = (*it)->Transform().Get();

                for (size_t i = 0; i < 4; ++i) {
                    for (size_t j = 0; j < 4; ++j) {
                        m_BufferData.emplace_back(model[i][j]);
                    }
                }

                m_BufferData.emplace_back((*it)->LifePercentage());

                m_ParticleCount++;
            } else {
                garbageCollector.emplace_back(it);
            }
        }

        for (auto it = garbageCollector.begin(); it != garbageCollector.end(); ++it) {
            m_Batch.erase(*it);
        }
    }

    void ParticleRenderer::OnRender(const params_t& params) {
        m_pShader->Bind();
        m_pShader->UpdateUniforms(m_pSystem);

        m_pBuffer->UpdateBufferData(vd::gl::eArrayBuffer, (m_BufferData.size() << 2), &m_BufferData[0], 0);
        m_pBuffer->DrawArrays(vd::gl::ePoints, m_ParticleCount);

        m_pShader->Unbind();
    }

    void ParticleRenderer::OnCleanUp() {
        m_pShader->CleanUp();    
        m_pBuffer->CleanUp();
    }

    bool ParticleRenderer::Precondition(const params_t& params) {
        if (m_pShader == nullptr || m_pSystem == nullptr) {
            return false;
        }

        return params.at("RenderingPass") == "Main";
    }

    void ParticleRenderer::Prepare() {
        vd::gl::Context::DepthMask(false);
        vd::gl::Context::CounterClockwiseFacing();

        if (m_pSystem->AdditiveBlending()) {
            vd::gl::Context::AdditiveBlending();
        } else {
            vd::gl::Context::AlphaBlending();
        }
    }

    void ParticleRenderer::Finish() {
        vd::gl::Context::Reset();
    }

}
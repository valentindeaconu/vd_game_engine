//
// Created by Vali on 4/3/2021.
//

#include "ParticleRenderer.hpp"

namespace mod::particles {
    ParticleRenderer::ParticleRenderer(vd::component::IEntityShaderPtr shader,
                                       vd::Consumer beforeExecution,
                                       vd::Consumer afterExecution) 
        : vd::component::IRenderer(std::move(shader), std::move(beforeExecution), std::move(afterExecution))
    {
    }

    void ParticleRenderer::Link() {
        m_pEventHandler = vd::ObjectOfType<vd::event::EventHandler>::Find();
        m_pContext = vd::ObjectOfType<vd::context::Context>::Find();
        m_pPlayer = vd::ObjectOfType<mod::player::Player>::Find();
    }

    void ParticleRenderer::Init() {
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

        m_pParticleSystem = std::make_shared<ParticleSystem>(75, 4.2f, 0.02f, 2.45f, "./resources/assets/particles/fire_8x8.png", 8);
        m_pParticleSystem->RandomizeScale(0.49f, 0.85f);
        m_pParticleSystem->RandomizeVelocity(-0.17f, 0.17f);
    }

    void ParticleRenderer::Update() {
        if (m_pEventHandler->KeyDown(vd::Key::eU)) {
            glm::vec3 pos = m_pPlayer->WorldTransform().Translation();

            m_Batch.emplace_back(
                std::make_shared<Particle>(pos, glm::vec3(0, 30, 0), 1, 1.50f, 0, 0.25f)
            );
        }

        ParticlePtrVec particles;
        m_pParticleSystem->GenerateParticles(m_pPlayer->WorldTransform().Translation(), m_pContext->FrameTime(), particles);

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

    void ParticleRenderer::Render(const params_t& params) {
        if (!IsReady()) {
            vd::Logger::warn("ParticleRenderer was not ready to render");
            return;
        }

        const auto& renderingPass = params.at("RenderingPass");

        if (renderingPass == "Main") {
            Prepare();

            m_pShader->Bind();
            m_pShader->UpdateUniforms(nullptr, 0, 0);

            m_pParticleSystem->TextureAtlas()->BindToUnit(0);
            m_pShader->SetUniform("uAtlasSampler.Atlas", 0);
            m_pShader->SetUniform("uAtlasSampler.Size", int(m_pParticleSystem->TextureAtlas()->Size()));

            m_pBuffer->UpdateBufferData(vd::gl::eArrayBuffer, (m_BufferData.size() << 2), &m_BufferData[0], 0);
            m_pBuffer->DrawArrays(vd::gl::ePoints, m_ParticleCount);

            m_pShader->Unbind();

            Finish();
        }
    }

    void ParticleRenderer::CleanUp() {
        m_pShader->CleanUp();    
        m_pBuffer->CleanUp();
    }

}
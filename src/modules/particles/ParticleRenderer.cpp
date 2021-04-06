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
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
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
        ParticlePtrVec particles;
        m_pSystem->GenerateParticles(m_pPlayer->WorldTransform().Translation(), m_pContext->FrameTime(), particles);

        for (auto& particle : particles) {
            if (m_pSystem->AdditiveBlending()) {
                // If additive blending is used, order of particles does not metter because
                // addition is commutative
                m_Batch.emplace_back(std::move(particle));
            } else { 
                const auto& cameraPosition = m_pCamera->Position();
                // Insertion Sort - decreasing order by square distance from the camera
                // This method is the fastest way since for 60 particles per second, particles array
                // will contain 0 or 1 particles, for 120 pps, 1 or 2, for 180 pps 3 or 4.
                auto it = m_Batch.begin();
                for (; it != m_Batch.end(); ++it) {
                    if (particle->SquareDistance(cameraPosition, true) > (*it)->SquareDistance()) {
                        break;
                    }
                }

                // If there are no particles in the batch, square distance will not be computed for the
                // very first particle, so we need to manually compute it
                if (it == m_Batch.begin()) {
                    particle->SquareDistance(cameraPosition, true);
                }

                // Batch is a linked list, so insertion will be realised in constant time
                m_Batch.insert(it, std::move(particle));
            }
        }

        m_BufferData.clear();
        m_BufferData.reserve(m_DataLength);
        m_ParticleCount = 0;

        std::vector<std::list<ParticlePtr>::iterator> garbageCollector;
        for (auto it = m_Batch.begin(); it != m_Batch.end(); ++it) {
            (*it)->Update(m_pContext->FrameTime());
            if ((*it)->Alive()) {
                glm::mat4 modelView = BuildModelViewMatrix(
                    (*it)->Position(),
                    (*it)->Scale(),
                    (*it)->Rotation(),
                    m_pCamera->ViewMatrix()
                );

                for (size_t i = 0; i < 4; ++i) {
                    for (size_t j = 0; j < 4; ++j) {
                        m_BufferData.emplace_back(modelView[i][j]);
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

    glm::mat4 ParticleRenderer::BuildModelViewMatrix(const glm::vec3& position, 
                                                     float scale, 
                                                     float rotation, 
                                                     const glm::mat4& view) 
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);

        // We need to make a ModelView matrix which contains no rotation after they are 
        // multiplied with each other (so that we can obtain a billboard effect - always facing the camera)
        model[0][0] = view[0][0];
        model[0][1] = view[1][0];
        model[0][2] = view[2][0];
        model[1][0] = view[0][1];
        model[1][1] = view[1][1];
        model[1][2] = view[2][1];
        model[2][0] = view[0][2];
        model[2][1] = view[1][2];
        model[2][2] = view[2][2];

        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 0, 1));
        model = glm::scale(model, glm::vec3(scale));

        return view * model;
    }
}
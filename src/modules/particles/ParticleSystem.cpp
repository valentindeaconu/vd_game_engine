//
// Created by Vali on 4/3/2021.
//

#include "ParticleSystem.hpp"

namespace mod::particles {

    ParticleSystem::ParticleSystem(float particlesPerSecond, float speed, float gravityComplient, float lifeLength, const std::string& texturePath, size_t size) 
        : m_ParticlesPerSecond(particlesPerSecond)
        , m_Speed(speed)
        , m_GravityComplient(gravityComplient)
        , m_LifeLength(lifeLength)
    {
        std::random_device rd{};
        m_Generator = std::mt19937({ rd() });
        m_Distribution = std::uniform_real_distribution<float>(0.0f, 1.0f);
        m_pTextureAtlas = vd::service::TextureService::CreateFromFile(texturePath, size, false);
    }

    void ParticleSystem::GenerateParticles(const glm::vec3& pointCenter, float frameTime, ParticlePtrVec& particles) {
        float particlesCount = m_ParticlesPerSecond * frameTime; 
        int count = int(glm::floor(particlesCount));
        float partialParticle = particlesCount - count;

        particles.clear();
        particles.reserve(count + 1);

        for (int i = 0; i < count; ++i) {
            particles.emplace_back(std::move(EmitParticle(pointCenter)));
        }

        if (RandomValue() < partialParticle) {
            particles.emplace_back(std::move(EmitParticle(pointCenter)));
        }
    }

    void ParticleSystem::GenerateParticles(const glm::vec3& pointCenter, float radius, float frameTime, ParticlePtrVec& particles) {
        float particlesCount = m_ParticlesPerSecond * frameTime; 
        int count = int(glm::floor(particlesCount));
        float partialParticle = particlesCount - count;

        particles.clear();
        particles.reserve(count + 1);


        for (int i = 0; i < count; ++i) {
            glm::vec3 position = pointCenter;
            position.x += radius * vd::math::Map01(RandomValue(), -1.0f, 1.0f);;
            position.z += radius * vd::math::Map01(RandomValue(), -1.0f, 1.0f);;
            particles.emplace_back(std::move(EmitParticle(position)));
        }

        if (RandomValue() < partialParticle) {
            glm::vec3 position = pointCenter;
            position.x += radius * vd::math::Map01(RandomValue(), -1.0f, 1.0f);;
            position.z += radius * vd::math::Map01(RandomValue(), -1.0f, 1.0f);;
            particles.emplace_back(std::move(EmitParticle(position)));
        }
    }

    void ParticleSystem::RandomizeVelocity(float low, float high) {
        m_VelocityInfo.Enabled = true;
        m_VelocityInfo.Low = low;
        m_VelocityInfo.High = high;
    }

    void ParticleSystem::RandomizeScale(float low, float high) {
        m_ScaleInfo.Enabled = true;
        m_ScaleInfo.Low = low;
        m_ScaleInfo.High = high;
    }

    void ParticleSystem::RandomizeRotation(float low, float high) {
        m_RotationInfo.Enabled = true;
        m_RotationInfo.Low = low;
        m_RotationInfo.High = high;
    }

    const vd::gl::Texture2DPtr& ParticleSystem::TextureAtlas() const {
        return m_pTextureAtlas;
    }

    ParticlePtr ParticleSystem::EmitParticle(const glm::vec3& position) {
        glm::vec3 velocity(1.0f);

        if (m_VelocityInfo.Enabled) {
            velocity.x = vd::math::Map01(RandomValue(), m_VelocityInfo.Low, m_VelocityInfo.High);
            velocity.z = vd::math::Map01(RandomValue(), m_VelocityInfo.Low, m_VelocityInfo.High);
        } else {
            velocity.x = vd::math::Map01(RandomValue(), -1.0f, 1.0f);
            velocity.z = vd::math::Map01(RandomValue(), -1.0f, 1.0f);
        }

        velocity = glm::normalize(velocity) * m_Speed;

        float rotation = 0.0f;
        if (m_RotationInfo.Enabled) {
            float v = RandomValue();
            rotation = vd::math::Map01(v, m_RotationInfo.Low, m_RotationInfo.High);
        }

        float scale = 1.0f;
        if (m_ScaleInfo.Enabled) {
            float v = RandomValue();
            scale = vd::math::Map01(v, m_ScaleInfo.Low, m_ScaleInfo.High);
        }

        return std::make_shared<Particle>(position, velocity, m_GravityComplient, m_LifeLength, rotation, scale);
    }

    float ParticleSystem::RandomValue() {
        return m_Distribution(m_Generator);
    }

}
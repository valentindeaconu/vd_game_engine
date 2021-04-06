//
// Created by Vali on 4/3/2021.
//

#ifndef VDGE_PARTICLESYSTEM_HPP
#define VDGE_PARTICLESYSTEM_HPP

#include <random>

#include <engine/api/gl/Texture.hpp>
#include <engine/service/TextureService.hpp>

#include "Particle.hpp"

namespace mod::particles {

    class ParticleSystem {
    public:
        ParticleSystem(float particlesPerSecond, float speed, float gravityComplient, float lifeLength, const std::string& texturePath, size_t size);

        void GenerateParticles(const glm::vec3& pointCenter, float frameTime, ParticlePtrVec& particles);
        void GenerateParticles(const glm::vec3& pointCenter, float radius, float frameTime, ParticlePtrVec& particles);

        void RandomizeVelocity(float low, float high);
        void RandomizeScale(float low, float high);
        void RandomizeRotation(float low, float high);

        [[nodiscard]] bool& AdditiveBlending();
        [[nodiscard]] const vd::gl::Texture2DPtr& TextureAtlas() const;
    private:
        ParticlePtr EmitParticle(const glm::vec3& position);
        float RandomValue();

        bool m_AdditiveBlending;

        float m_ParticlesPerSecond;
        float m_Speed;
        float m_GravityComplient;
        float m_LifeLength;

        struct {
            bool Enabled;
            float Low;
            float High;
        } m_ScaleInfo, m_RotationInfo, m_VelocityInfo;

        // Random generator
        std::mt19937 m_Generator;
        std::uniform_real_distribution<float> m_Distribution;

        vd::gl::Texture2DPtr    m_pTextureAtlas;
    };
    typedef std::shared_ptr<ParticleSystem> ParticleSystemPtr;

}

#endif //VDGE_PARTICLESYSTEM_HPP
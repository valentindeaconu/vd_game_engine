//
// Created by Vali on 4/3/2021.
//

#ifndef VDGE_PARTICLE_HPP
#define VDGE_PARTICLE_HPP

#include <memory>
#include <vector>

#include <engine/defines/Physics.hpp>
#include <engine/math/Transform.hpp>
#include <engine/math/Utils.hpp>

namespace mod::particles {
    class Particle {
    public:
        Particle(const glm::vec3& position, const glm::vec3& velocity, float gravityEffect, float lifeLength, float rotation, float scale);

        void Update(float frameTime);

        [[nodiscard]] bool Alive() const;

        [[nodiscard]] const glm::vec3& Position() const;
        [[nodiscard]] const glm::vec3& Velocity() const;
        [[nodiscard]] float GravityEffect() const;
        [[nodiscard]] float LifeLength() const;
        [[nodiscard]] float LifePercentage() const;
        [[nodiscard]] const vd::math::Transform& Transform() const;
    private:
        glm::vec3               m_Position;
        glm::vec3               m_Velocity;
        float                   m_GravityEffect;
        float                   m_ElapsedTime;
        float                   m_LifeLength;
        float                   m_LifePercentage;
        vd::math::Transform     m_Transform;
    };
    typedef std::shared_ptr<Particle>    ParticlePtr;
    typedef std::vector<ParticlePtr>     ParticlePtrVec;
}

#endif //VDGE_PARTICLE_HPP
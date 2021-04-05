//
// Created by Vali on 4/3/2021.
//

#include "Particle.hpp"

namespace mod::particles {
    Particle::Particle(const glm::vec3& position, const glm::vec3& velocity, float gravityEffect, float lifeLength, float rotation, float scale)
        : m_Position(position)
        , m_Velocity(velocity)
        , m_GravityEffect(gravityEffect)
        , m_LifeLength(lifeLength)
        , m_ElapsedTime(0.0f)
    {
        // m_Transform.ZAxisRotationAngle() = vd::math::ReduceAngle(rotation);
        m_Transform.Scale() = glm::vec3(scale);
    }

    void Particle::Update(float frameTime) {
        m_ElapsedTime += frameTime;
        m_LifePercentage = (m_ElapsedTime / m_LifeLength);
        
        if (Alive()) {
            m_Velocity.y += vd::g_kGravitySpeed * m_GravityEffect * frameTime;
            m_Position += (m_Velocity * frameTime);
            m_Transform.Translation() = glm::vec3(m_Position);
        }
    }

    bool Particle::Alive() const {
        return m_ElapsedTime < m_LifeLength;
    }

    const glm::vec3& Particle::Position() const {
        return m_Position;
    }

    const glm::vec3& Particle::Velocity() const {
        return m_Velocity;
    }

    float Particle::GravityEffect() const {
        return m_GravityEffect;
    }

    float Particle::LifeLength() const {
        return m_LifeLength;
    }

    float Particle::LifePercentage() const {
        return m_LifePercentage;
    }

    const vd::math::Transform& Particle::Transform() const {
        return m_Transform;
    }


}
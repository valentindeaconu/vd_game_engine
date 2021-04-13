//
// Created by Vali on 4/3/2021.
//

#include "Particle.hpp"

namespace mod::particles {
    Particle::Particle(const glm::vec3& position, const glm::vec3& velocity, float gravityEffect, float lifeLength, float rotation, float scale)
        : m_Position(position)
        , m_Scale(scale)
        , m_Rotation(rotation)
        , m_Velocity(velocity)
        , m_GravityEffect(gravityEffect)
        , m_LifeLength(lifeLength)
        , m_SquareDistance(-1.0f)
        , m_ElapsedTime(0.0f)
    {
    }

    void Particle::Update(float frameTime) {
        m_ElapsedTime += frameTime;
        m_LifePercentage = (m_ElapsedTime / m_LifeLength);
        
        if (Alive()) {
            m_Velocity.y += vd::g_kGravitySpeed * m_GravityEffect * frameTime;
            m_Position += (m_Velocity * frameTime);
        }
    }

    float Particle::SquareDistance(const glm::vec3& referencePoint, bool reload) {
        if (reload || m_SquareDistance < 0.0f) {
            m_SquareDistance = glm::length2(glm::vec3(referencePoint - m_Position));
        }

        return m_SquareDistance;
    }

    bool Particle::Alive() const {
        return m_ElapsedTime < m_LifeLength;
    }

    const glm::vec3& Particle::Position() const {
        return m_Position;
    }

    float Particle::Scale() const {
        return m_Scale;
    }

    float Particle::Rotation() const {
        return m_Rotation;
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

}
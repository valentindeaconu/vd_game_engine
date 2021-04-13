//
// Created by Vali on 4/3/2021.
//

#ifndef VDGE_PARTICLE_HPP
#define VDGE_PARTICLE_HPP

#include <memory>
#include <vector>
#include <glm/gtx/norm.hpp> // for glm::length2

#include <engine/defines/Physics.hpp>
#include <engine/math/Utils.hpp>

namespace mod::particles {
    class Particle {
    public:
        Particle(const glm::vec3& position, const glm::vec3& velocity, float gravityEffect, float lifeLength, float rotation, float scale);

        void Update(float frameTime);

        // Lazy load - compute square distance from a given point at first call
        // then return the previous computed value
        // If reload parameter is set to true, the value is recomputed
        float SquareDistance(const glm::vec3& referencePoint = glm::vec3(0), bool reload = false);

        [[nodiscard]] bool Alive() const;

        [[nodiscard]] const glm::vec3& Position() const;
        [[nodiscard]] float Scale() const;
        [[nodiscard]] float Rotation() const;

        [[nodiscard]] const glm::vec3& Velocity() const;
        [[nodiscard]] float GravityEffect() const;
        [[nodiscard]] float LifeLength() const;
        [[nodiscard]] float LifePercentage() const;
    private:
        glm::vec3               m_Position;
        float                   m_Scale;
        float                   m_Rotation;

        glm::vec3               m_Velocity;
        float                   m_GravityEffect;
        float                   m_ElapsedTime;
        float                   m_LifeLength;
        float                   m_LifePercentage;
        float                   m_SquareDistance;
    };
    typedef std::shared_ptr<Particle>    ParticlePtr;
    typedef std::vector<ParticlePtr>     ParticlePtrVec;
}

#endif //VDGE_PARTICLE_HPP
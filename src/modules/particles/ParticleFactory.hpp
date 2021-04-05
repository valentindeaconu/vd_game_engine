//
// Created by Vali on 4/3/2021.
//

#ifndef VDGE_PARTICLEFACTORY_HPP
#define VDGE_PARTICLEFACTORY_HPP

#include <engine/component/IModuleFactory.hpp>

#include <engine/injector/CreateAndStore.hpp>
#include <engine/defines/Types.hpp>

#include "Particle.hpp"
#include "ParticleShader.hpp"
#include "ParticleRenderer.hpp"

namespace mod::particles {

    class ParticleFactory : public vd::component::IModuleFactory {
    public:
        void Create(const vd::EnginePtr& pEngine) override;
    };
    typedef std::shared_ptr<ParticleFactory> ParticleFactoryPtr;

}

#endif //VDGE_PARTICLEFACTORY_HPP
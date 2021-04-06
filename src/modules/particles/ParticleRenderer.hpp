//
// Created by Vali on 4/3/2021.
//

#include <memory>
#include <list>

#include <engine/component/IRenderer.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/context/Context.hpp>
#include <engine/event/EventHandler.hpp>
#include <modules/player/Player.hpp>

#include <engine/api/gl/Context.hpp>

#include <modules/water/WaterRenderer.hpp>

#include "Particle.hpp"
#include "ParticleSystem.hpp"
#include "ParticleShader.hpp"

namespace mod::particles {
    class ParticleRenderer : public vd::component::IRenderer, public vd::injector::Injectable {
    public:
        static const int kPriority = water::WaterRenderer::kPriority + 1;

        ParticleRenderer(ParticleSystemPtr particleSystem);

        void Link() override;

        void OnInit() override;
        void OnUpdate() override;
        void OnRender(const params_t& params) override;
        void OnCleanUp() override;
    private:
        bool Precondition(const params_t& params) override;
        void Prepare() override;
        void Finish() override;

        size_t                      m_DataLength;
        size_t                      m_ParticleCount;

        vd::gl::BufferPtr           m_pBuffer;

        std::list<ParticlePtr>      m_Batch;
        std::vector<float>          m_BufferData;

        vd::context::ContextPtr     m_pContext;
        vd::event::EventHandlerPtr  m_pEventHandler;
        mod::player::PlayerPtr      m_pPlayer;

        ParticleShaderPtr           m_pShader;
        ParticleSystemPtr           m_pSystem;
    };
    typedef std::shared_ptr<ParticleRenderer>	ParticleRendererPtr;
}
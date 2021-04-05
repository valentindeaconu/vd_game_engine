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

#include <modules/water/WaterRenderer.hpp>

#include "Particle.hpp"
#include "ParticleSystem.hpp"

namespace mod::particles {
    class ParticleRenderer : public vd::component::IRenderer, public vd::injector::Injectable {
    public:
        static const int kPriority = water::WaterRenderer::kPriority + 1;

        ParticleRenderer(vd::component::IEntityShaderPtr shader,
                         vd::Consumer beforeExecution = vd::g_kEmptyConsumer,
                         vd::Consumer afterExecution = vd::g_kEmptyConsumer);
        
        void Link() override;

        void Init() override;
        void Update() override;
        void Render(const params_t& params) override;
        void CleanUp() override;
    private:
        size_t                  m_DataLength;
        size_t                  m_ParticleCount;

        vd::gl::BufferPtr       m_pBuffer;

        std::list<ParticlePtr>  m_Batch;
        std::vector<float>      m_BufferData;

        vd::context::ContextPtr     m_pContext;
        vd::event::EventHandlerPtr  m_pEventHandler;
        mod::player::PlayerPtr      m_pPlayer;

        ParticleSystemPtr m_pParticleSystem;
    };
    typedef std::shared_ptr<ParticleRenderer>	ParticleRendererPtr;
}
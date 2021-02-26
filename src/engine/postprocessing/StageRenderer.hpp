//
// Created by Vali on 2/26/2021.
//

#ifndef VDGE_STAGERENDERER_HPP
#define VDGE_STAGERENDERER_HPP

#include <engine/component/IRenderer.hpp>
#include <engine/object/primitive/Quad2D.hpp>

namespace vd::postprocessing {
    class StageRenderer : public vd::component::IRenderer {
        static const int kPriority = kDefaultPriority + 99;

        StageRenderer(vd::Consumer beforeExecution = vd::g_kEmptyConsumer, vd::Consumer afterExecution = vd::g_kEmptyConsumer);



        void Init() override;
        void Update() override;
        void Render(const params_t& params) override;
        void CleanUp() override;

    private:
        bool IsReady() override;

        struct StageShader {
            StagePtr Stage;
            IStageShaderPtr Shader;
        };
        std::vector<StageShader> m_Stages;

        vd::object::primitive::Quad2DPtr m_pQuad;
    };
    typedef std::shared_ptr<StageRenderer>	StageRendererPtr;
}


#endif //VDGE_STAGERENDERER_HPP

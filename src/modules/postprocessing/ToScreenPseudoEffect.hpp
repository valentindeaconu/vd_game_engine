//
// Created by Vali on 2/26/2021.
//

#ifndef VDGE_TOSCREENPSEUDOEFFECT_HPP
#define VDGE_TOSCREENPSEUDOEFFECT_HPP

#include <engine/component/RenderingEffect.hpp>
#include <engine/component/IRenderingEffectShader.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/context/Context.hpp>

namespace mod::postprocessing {
    class ToScreenPseudoEffect : public vd::component::ConcreteEffect {
    public:
        explicit ToScreenPseudoEffect(vd::component::FrameBuffersResolver inputGetter);
        void Init(vd::Dimension dimension) override;
    };
    typedef std::shared_ptr<ToScreenPseudoEffect>   ToScreenPseudoEffectPtr;

    class ToScreenShader : public vd::component::IRenderingEffectShader, public vd::injector::Injectable {
    public:
        ToScreenShader();

        void Link() override;

        void InitUniforms(vd::component::IRenderingEffectPtr pEffect) override;
        void UpdateUniforms(vd::component::IRenderingEffectPtr pEffect) override;

    protected:
        void AddUniforms() override;

    private:
        vd::context::ContextPtr m_pContext;
    };
    typedef std::shared_ptr<ToScreenShader>   ToScreenShaderPtr;
}


#endif //VDGE_TOSCREENPSEUDOEFFECT_HPP

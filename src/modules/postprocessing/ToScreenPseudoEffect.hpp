//
// Created by Vali on 2/26/2021.
//

#ifndef VDGE_TOSCREENPSEUDOEFFECT_HPP
#define VDGE_TOSCREENPSEUDOEFFECT_HPP

#include <engine/component/RenderingEffect.hpp>
#include <engine/component/IRenderingEffectShader.hpp>

namespace mod::postprocessing {
    class ToScreenPseudoEffect : public vd::component::ConcreteEffect {
    public:
        explicit ToScreenPseudoEffect(vd::component::FrameBuffersResolver inputGetter);
        void Init(vd::Dimension dimension) override;
    };
    typedef std::shared_ptr<ToScreenPseudoEffect>   ToScreenPseudoEffectPtr;

    class ToScreenShader : public vd::component::IRenderingEffectShader {
    public:
        ToScreenShader();

        void InitUniforms(vd::component::IRenderingEffectPtr pEffect) override;
        void UpdateUniforms(vd::component::IRenderingEffectPtr pEffect) override;

    protected:
        void AddUniforms() override;
    };
    typedef std::shared_ptr<ToScreenShader>   ToScreenShaderPtr;
}


#endif //VDGE_TOSCREENPSEUDOEFFECT_HPP

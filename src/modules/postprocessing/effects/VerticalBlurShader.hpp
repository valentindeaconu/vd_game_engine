//
// Created by Vali on 2/24/2021.
//

#ifndef VDGE_VERTICALBLURSHADER_HPP
#define VDGE_VERTICALBLURSHADER_HPP

#include <engine/component/IRenderingEffectShader.hpp>
#include <engine/component/RenderingEffect.hpp>

namespace mod::postprocessing {
    class VerticalBlurShader : public vd::component::IRenderingEffectShader {
    public:
        VerticalBlurShader();

        void InitUniforms(vd::component::IRenderingEffectPtr pEffect) override;
        void UpdateUniforms(vd::component::IRenderingEffectPtr pEffect) override;

    protected:
        void AddUniforms() override;
    };
    typedef std::shared_ptr<VerticalBlurShader>   VerticalBlurShaderPtr;
}


#endif //VDGE_VERTICALBLURSHADER_HPP

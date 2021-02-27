//
// Created by Vali on 2/24/2021.
//

#ifndef VDGE_HORIZONTALBLURSHADER_HPP
#define VDGE_HORIZONTALBLURSHADER_HPP

#include <engine/component/IRenderingEffectShader.hpp>
#include <engine/component/RenderingEffect.hpp>

namespace mod::postprocessing {
    class HorizontalBlurShader : public vd::component::IRenderingEffectShader {
    public:
        HorizontalBlurShader();

        void InitUniforms(vd::component::IRenderingEffectPtr pEffect) override;
        void UpdateUniforms(vd::component::IRenderingEffectPtr pEffect) override;

    protected:
        void AddUniforms() override;
    };
    typedef std::shared_ptr<HorizontalBlurShader>   HorizontalBlurShaderPtr;
}

#endif //VDGE_HORIZONTALBLURSHADER_HPP

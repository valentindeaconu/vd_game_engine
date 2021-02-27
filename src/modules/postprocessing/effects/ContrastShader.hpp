//
// Created by Vali on 2/19/2021.
//

#ifndef VDGE_CONTRASTSHADER_HPP
#define VDGE_CONTRASTSHADER_HPP

#include <engine/component/RenderingEffect.hpp>
#include <engine/component/IRenderingEffectShader.hpp>

namespace mod::postprocessing {
    class ContrastShader : public vd::component::IRenderingEffectShader {
    public:
        ContrastShader();
        void InitUniforms(vd::component::IRenderingEffectPtr pEffect) override;
        void UpdateUniforms(vd::component::IRenderingEffectPtr pEffect) override;

    protected:
        void AddUniforms() override;
    };
    typedef std::shared_ptr<ContrastShader>   ContrastShaderPtr;

}


#endif //VDGE_CONTRASTSHADER_HPP

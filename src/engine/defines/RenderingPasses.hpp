//
// Created by Vali on 4/13/2021.
//

#ifndef VDGE_RENDERINGPASSES_HPP
#define VDGE_RENDERINGPASSES_HPP

namespace vd {
    enum class RenderingPassType {
        eUpdate = 0,
        eShadow,
        eReflection,
        eRefraction,
        eMain,
        ePostProcessing,
        eGUI
    };
}

#endif //VDGE_RENDERINGPASSES_HPP
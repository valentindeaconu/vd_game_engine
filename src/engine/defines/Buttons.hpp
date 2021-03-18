//
// Created by Vali on 3/17/2021.
//

#ifndef VDGE_BUTTONS_HPP
#define VDGE_BUTTONS_HPP

namespace vd {

    struct Button {
        enum Code {
            eUnknown = -1,
            eLeft = 0,
            eRight,
            eMiddle,
            eButton4,
            eButton5,
            eButton6,
            eButton7,
            eButton8
        };
    };

}

#endif //VDGE_BUTTONS_HPP
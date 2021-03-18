//
// Created by Vali on 3/17/2021.
//

#ifndef VDGE_ACTIONS_HPP
#define VDGE_ACTIONS_HPP

namespace vd {

    struct Action {
        enum Code {
            ePress = 0,
            eRepeat,
            eRelease,
            eUnknown
        };
    };

}

#endif //VDGE_ACTIONS_HPP
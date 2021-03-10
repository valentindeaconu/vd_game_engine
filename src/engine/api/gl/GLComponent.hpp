//
// Created by Vali on 3/10/2021.
//

#ifndef VDGE_GLCOMPONENT_HPP
#define VDGE_GLCOMPONENT_HPP

namespace vd::gl {

    class GLComponent {
    public:
        virtual void Create() = 0;
        virtual void CleanUp() = 0;
    };

}

#endif //VDGE_GLCOMPONENT_HPP

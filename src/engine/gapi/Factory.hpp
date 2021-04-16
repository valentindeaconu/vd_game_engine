//
// Created by vali on 4/16/21.
//

#ifndef VDGE_FACTORY_HPP
#define VDGE_FACTORY_HPP

#include "Buffer.hpp"
#include "Program.hpp"

namespace vd::gapi {
    class Factory {
    public:
        static BufferPtr CreateBuffer();

        static ProgramPtr CreateProgram();
    };
}


#endif //VDGE_FACTORY_HPP

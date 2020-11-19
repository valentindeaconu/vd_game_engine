//
// Created by Vali on 11/19/2020.
//

#include "Injectable.hpp"

namespace vd::injector {

    Injectable::Injectable() {
        ObjectOfType<Linker>::Find()->Subscribe(this);
    }

}

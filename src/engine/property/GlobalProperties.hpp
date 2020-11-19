//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_GLOBALPROPERTIES_HPP
#define VD_GAME_ENGINE_GLOBALPROPERTIES_HPP

#include "Properties.hpp"

namespace vd::property {
    class GlobalProperties : public property::Properties {
    public:
        GlobalProperties() = default;
        ~GlobalProperties() = default;
    };
    typedef std::shared_ptr<GlobalProperties>  GlobalPropertiesPtr;
}


#endif //VD_GAME_ENGINE_GLOBALPROPERTIES_HPP

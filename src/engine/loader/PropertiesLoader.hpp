//
// Created by Vali on 11/14/2020.
//

#ifndef VD_GAME_ENGINE_PROPERTIESLOADER_HPP
#define VD_GAME_ENGINE_PROPERTIESLOADER_HPP

#include <engine/property/Properties.hpp>
#include <engine/injector/ObjectOfType.hpp>
#include <engine/exception/Exceptions.hpp>

#include "FileLoader.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

namespace vd::loader {
    class PropertiesLoader {
    public:
        static void Load(const std::string& path, property::PropertiesPtr& output);
        static property::PropertiesPtr Load(const std::string& path);
    };
}


#endif //VD_GAME_ENGINE_PROPERTIESLOADER_HPP

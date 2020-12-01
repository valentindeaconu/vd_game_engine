//
// Created by Vali on 11/14/2020.
//

#ifndef VD_GAME_ENGINE_FILELOADER_HPP
#define VD_GAME_ENGINE_FILELOADER_HPP

#include <memory>
#include <string>

#include <engine/injector/ObjectOfType.hpp>

#include <boost/algorithm/string.hpp>

namespace vd::loader {
    namespace impl {
        class IFileLoader {
        public:
            virtual void Load(const std::string& path, std::string& output) = 0;
        };
        typedef std::shared_ptr<IFileLoader> IFileLoaderPtr;
    }

    class FileLoader {
    public:
        static std::string Load(const std::string& path);
        static void Load(const std::string& path, std::string& output);
        static void Load(const std::string& path, std::vector<std::string>& lines);
    };
}


#endif //VD_GAME_ENGINE_FILELOADER_HPP

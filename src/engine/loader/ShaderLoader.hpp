//
// Created by Vali on 11/14/2020.
//

#ifndef VD_GAME_ENGINE_SHADERLOADER_HPP
#define VD_GAME_ENGINE_SHADERLOADER_HPP

#include <memory>
#include <string>

#include <engine/injector/ObjectOfType.hpp>

namespace vd::loader {
    namespace impl {
        class IShaderLoader {
        public:
            virtual void Load(const std::string& path, std::string& output) = 0;
        };
        typedef std::shared_ptr<IShaderLoader>  IShaderLoaderPtr;
    }

    class ShaderLoader {
    public:
        static std::string Load(const std::string& path);
        static void Load(const std::string& path, std::string& output);
    };
}


#endif //VD_GAME_ENGINE_SHADERLOADER_HPP

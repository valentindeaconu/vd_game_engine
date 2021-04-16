//
// Created by Vali on 11/14/2020.
//

#include "ShaderLoader.hpp"

namespace vd::loader {

    std::string ShaderLoader::Load(const std::string& path) {
        impl::IShaderLoaderPtr impl = ObjectOfType<impl::IShaderLoader>::Find();

        impl->Prepare();

        std::string content;
        impl->Load(path, content);
        return content;
    }

    void ShaderLoader::Load(const std::string& path, std::string& output) {
        impl::IShaderLoaderPtr impl = ObjectOfType<impl::IShaderLoader>::Find();

        impl->Prepare();
        impl->Load(path, output);
    }

}
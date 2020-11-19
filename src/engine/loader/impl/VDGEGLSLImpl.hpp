//
// Created by Vali on 11/14/2020.
//

#ifndef VD_GAME_ENGINE_VDGEGLSLIMPL_HPP
#define VD_GAME_ENGINE_VDGEGLSLIMPL_HPP

#include <engine/loader/ShaderLoader.hpp>
#include <engine/loader/PropertiesLoader.hpp>

#include <fstream>
#include <unordered_map>
#include <filesystem>

#include "FileLoader.hpp"

namespace vd::loader::impl {
    class VDGEGLSLImpl : public IShaderLoader {
    public:
        VDGEGLSLImpl();
        ~VDGEGLSLImpl();

        void Load(const std::string& path, std::string& output) override;
    private:
        std::string m_IncludeDirectory;
        std::unordered_map<std::string, std::string> m_ConstantMap;

    };
    typedef std::shared_ptr<VDGEGLSLImpl>   VDGEGLSLImplPtr;
}


#endif //VD_GAME_ENGINE_VDGEGLSLIMPL_HPP

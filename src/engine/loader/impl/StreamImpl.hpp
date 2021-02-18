//
// Created by Vali on 11/14/2020.
//

#ifndef VD_GAME_ENGINE_STREAMIMPL_HPP
#define VD_GAME_ENGINE_STREAMIMPL_HPP

#include <engine/loader/FileLoader.hpp>

#include <engine/exception/Exceptions.hpp>

#include <fstream>

namespace vd::loader::impl {
    class StreamImpl : public IFileLoader {
    public:
        void Load(const std::string& path, std::string& output) override;
    };
    typedef std::shared_ptr<StreamImpl> StreamImplPtr;
}


#endif //VD_GAME_ENGINE_STREAMIMPL_HPP

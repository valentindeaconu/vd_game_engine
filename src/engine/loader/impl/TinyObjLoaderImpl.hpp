//
// Created by Vali on 11/13/2020.
//

#ifndef VD_GAME_ENGINE_TINYOBJLOADERIMPL_HPP
#define VD_GAME_ENGINE_TINYOBJLOADERIMPL_HPP

#include <engine/loader/ObjectLoader.hpp>
#include <engine/service/TextureService.hpp>

#include <memory>

namespace vd::loader::impl {
    class TinyObjLoaderImpl : public IObjectLoader {
    public:
        vd::model::MeshPtrVec Load(const std::string& path) override;
    };
    typedef std::shared_ptr<TinyObjLoaderImpl>  TinyObjLoaderImplPtr;
}


#endif //VD_GAME_ENGINE_TINYOBJLOADERIMPL_HPP

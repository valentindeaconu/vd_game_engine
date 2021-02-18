//
// Created by Vali on 2/17/2021.
//

#ifndef VDGE_FREETYPEIMPL_HPP
#define VDGE_FREETYPEIMPL_HPP

#include <engine/loader/FontLoader.hpp>

#include <engine/exception/Exceptions.hpp>

#include <engine/service/TextureService.hpp>

namespace vd::loader::impl {
    class FreeTypeImpl : public IFontLoader {
    public:
        void Load(const std::string& path, uint32_t fontSize, model::FontPtr& output) override;
    };
    typedef std::shared_ptr<FreeTypeImpl> FreeTypeImplPtr;
}


#endif //VDGE_FREETYPEIMPL_HPP

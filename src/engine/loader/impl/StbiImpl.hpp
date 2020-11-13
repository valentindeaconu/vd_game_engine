//
// Created by Vali on 11/13/2020.
//

#ifndef VD_GAME_ENGINE_STBIIMPL_HPP
#define VD_GAME_ENGINE_STBIIMPL_HPP

#include <engine/loader/ImageLoader.hpp>

namespace vd::loader::impl {
    class StbiImpl : public IImageLoader {
    public:
        LoadedImage Load(const std::string& path) override;
        void Release(const LoadedImage& loadedImage) override;
    };
    typedef std::shared_ptr<StbiImpl>   StbiImplPtr;
}


#endif //VD_GAME_ENGINE_STBIIMPL_HPP

//
// Created by Vali on 10/6/2020.
//

#ifndef VD_GAME_ENGINE_IMAGEHELPER_HPP
#define VD_GAME_ENGINE_IMAGEHELPER_HPP

#include <engine/foundation/img/Image.hpp>
#include <engine/logger/Logger.hpp>

#include <glm/glm.hpp>


namespace vd::img {
    class ImageHelper {
    public:
        template <typename T>
        static Pixel<T> texture(const Image<T>& image, const glm::vec2& uv);
    };
}


#endif //VD_GAME_ENGINE_IMAGEHELPER_HPP

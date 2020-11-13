//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_IMAGE_HPP
#define VD_GAME_ENGINE_IMAGE_HPP

#include <cstdint>
#include <vector>
#include <memory>
#include <cassert>

#include <glm/glm.hpp>

#include <engine/misc/Types.hpp>
#include <engine/math/Interpolator.hpp>

#include <engine/logger/Logger.hpp>

namespace vd::model {
    enum ImageFormat {
        eRGBA = 0,
        eBGRA,
        eRGB,
        eBGR,
        eR
    };

    template<typename T, ImageFormat F = ImageFormat::eRGBA>
    class Image {
    public:
        Image();
        Image(size_t width, size_t height);
        Image(const Image<T, F>& other);
        Image(Image<T, F>&& other) noexcept;
        ~Image();

        void Reverse();

        template <typename R> R Get(size_t i, size_t j) const;
        template <typename R, math::Interpolation I = math::Interpolation::eBilinear> R Get(const glm::vec2& uv) const;

        vd::Dimension& Dimension();
        std::vector<T>& Data();

        [[nodiscard]] uint64_t Width() const;
        [[nodiscard]] uint64_t Height() const;
    private:
        [[nodiscard]] bool Inside(int i, int j) const;

        vd::Dimension m_Dimension;
        std::vector<T> m_Data;
    };

    template <typename T, ImageFormat F = ImageFormat::eRGBA>
    using ImagePtr = std::shared_ptr<Image<T, F>>;

}

#endif //VD_GAME_ENGINE_IMAGE_HPP

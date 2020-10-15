//
// Created by Vali on 10/6/2020.
//

#include "ImageHelper.hpp"

namespace vd::img {
    // Template instantiation
    template Pixel<uint8_t>     ImageHelper::texture(const Image<uint8_t>&, const glm::vec2&);
    template Pixel<uint32_t>    ImageHelper::texture(const Image<uint32_t>&, const glm::vec2&);
    template Pixel<float>       ImageHelper::texture(const Image<float>&, const glm::vec2&);

    template<typename T>
    Pixel<T> ImageHelper::texture(const Image<T>& image, const glm::vec2& uv) {
        glm::vec2 uvCoords = uv;

        if (uvCoords.x < 0.0f || uvCoords.x > 1.0f || uvCoords.y < 0.0f || uvCoords.y > 1.0f) {
            vd::Logger::warn("UV coords exceed image, down sampling to accepted values");

            uvCoords.x -= std::floor(uvCoords.x);
            uvCoords.y -= std::floor(uvCoords.y);
        }

        float uv_x = uvCoords.x * (image.width - 1);
        float uv_y = uvCoords.y * (image.height - 1);

        int uv_x_0 = int(std::floor(uv_x));
        int uv_y_0 = int(std::floor(uv_y));
        int uv_x_1 = uv_x_0 + 1;
        int uv_y_1 = uv_y_0 + 1;

        if ((std::abs(uv_x - std::floor(uv_x)) < std::numeric_limits<float>::epsilon() &&
            std::abs(uv_y - std::floor(uv_y)) < std::numeric_limits<float>::epsilon()) ||
            (uv_x_1 >= image.width || uv_y_1 >= image.height))
        {
            return image.at(uv_x_0, uv_y_0);
        }

        Pixel<T> v0 = image.at(uv_x_0, uv_y_0, eRGBA);
        Pixel<T> v1 = image.at(uv_x_0, uv_y_1, eRGBA);
        Pixel<T> v2 = image.at(uv_x_1, uv_y_0, eRGBA);
        Pixel<T> v3 = image.at(uv_x_1, uv_y_1, eRGBA);

        float frac_x = uv_x - float(uv_x_0);
        float frac_y = uv_y - float(uv_y_0);

        Pixel<T> v01(
            glm::mix(v0.r, v1.r, frac_x),
            glm::mix(v0.g, v1.g, frac_x),
            glm::mix(v0.b, v1.b, frac_x),
            glm::mix(v0.a, v1.a, frac_x)
        );

        Pixel<T> v23(
            glm::mix(v2.r, v3.r, frac_x),
            glm::mix(v2.g, v3.g, frac_x),
            glm::mix(v2.b, v3.b, frac_x),
            glm::mix(v2.a, v3.a, frac_x)
        );

        return Pixel<T>(
            glm::mix(v01.r, v23.r, frac_y),
            glm::mix(v01.g, v23.g, frac_y),
            glm::mix(v01.b, v23.b, frac_y),
            glm::mix(v01.a, v23.a, frac_y)
        );
    }
}
//
// Created by Vali on 11/11/2020.
//

#include "Image.hpp"

namespace vd::model {
    // Forward declarations
    template class Image<uint8_t, eR>;
    template class Image<uint8_t, eRGB>;
    template class Image<uint8_t, eBGR>;
    template class Image<uint8_t, eRGBA>;
    template class Image<uint8_t, eBGRA>;

    template class Image<uint32_t, eR>;
    template class Image<uint32_t, eRGB>;
    template class Image<uint32_t, eBGR>;
    template class Image<uint32_t, eRGBA>;
    template class Image<uint32_t, eBGRA>;

    template class Image<float, eR>;
    template class Image<float, eRGB>;
    template class Image<float, eBGR>;
    template class Image<float, eRGBA>;
    template class Image<float, eBGRA>;

    // Impl
    template<typename T, ImageFormat F>
    Image<T, F>::Image()
        : m_Dimension(0, 0)
        , m_Data()
    {
    }

    template<typename T, ImageFormat F>
    Image<T, F>::Image(size_t width, size_t height)
        : m_Dimension(width, height)
        , m_Data()
    {
    }

    template<typename T, ImageFormat F>
    Image<T, F>::Image(const Image<T, F>& other)
        : m_Dimension(other.m_Dimension)
        , m_Data(other.m_Data)
    {
    }

    template<typename T, ImageFormat F>
    Image<T, F>::Image(Image<T, F>&& other) noexcept
        : m_Dimension(std::move(other.m_Dimension))
        , m_Data(std::move(other.m_Data))
    {
    }

    template<typename T, ImageFormat F> Image<T, F>::~Image() = default;

    template<typename T, ImageFormat F>
    void Image<T, F>::Reverse() {
        uint8_t channels;

        switch (F) {
            case eRGBA:
            case eBGRA:
                channels = 4;
                break;
            case eRGB:
            case eBGR:
                channels = 3;
                break;
            default:
            case eR:
                channels = 1;
                break;
        }

        int width_in_bytes = m_Dimension.width * channels;

        T* top = NULL;
        T* bottom = NULL;
        T temp = T(0);

        int half_height = m_Dimension.height >> 1;

        T* image_data = &m_Data[0];

        for (int row = 0; row < half_height; row++) {
            top = image_data + row * width_in_bytes;
            bottom = image_data + (m_Dimension.width - row - 1) * width_in_bytes;
            for (int col = 0; col < width_in_bytes; col++) {
                temp = *top;
                *top = *bottom;
                *bottom = temp;
                top++;
                bottom++;
            }
        }
    }

    /// Float data, RGBA format, 4 channels
    template <>
    template <>
    glm::vec4 Image<float, ImageFormat::eRGBA>::Get(size_t i, size_t j) const {
        if (!Inside(i, j)) {
            throw std::out_of_range("requested index is out of range");
        }

        size_t startIndex = (i * m_Dimension.width + j) * 4;

        glm::vec4 p;
        p.r = m_Data[startIndex];
        p.g = m_Data[startIndex + 1];
        p.b = m_Data[startIndex + 2];
        p.a = m_Data[startIndex + 3];
        return p;
    }

    /// Float data, BGRA format, 4 channels
    template <>
    template <>
    glm::vec4 Image<float, ImageFormat::eBGRA>::Get(size_t i, size_t j) const {
        if (!Inside(i, j)) {
            throw std::out_of_range("requested index is out of range");
        }

        size_t startIndex = (i * m_Dimension.width + j) * 4;

        glm::vec4 p;
        p.r = m_Data[startIndex + 2];
        p.g = m_Data[startIndex + 1];
        p.b = m_Data[startIndex];
        p.a = m_Data[startIndex + 3];
        return p;
    }

    /// Float data, RGB format, 3 channels
    template <>
    template <>
    glm::vec3 Image<float, ImageFormat::eRGB>::Get(size_t i, size_t j) const {
        if (!Inside(i, j)) {
            throw std::out_of_range("requested index is out of range");
        }

        size_t startIndex = (i * m_Dimension.width + j) * 3;

        glm::vec3 p;
        p.r = m_Data[startIndex];
        p.g = m_Data[startIndex + 1];
        p.b = m_Data[startIndex + 2];
        return p;
    }

    /// Float data, BGR format, 3 channels
    template <>
    template <>
    glm::vec3 Image<float, ImageFormat::eBGR>::Get(size_t i, size_t j) const {
        if (!Inside(i, j)) {
            throw std::out_of_range("requested index is out of range");
        }

        size_t startIndex = (i * m_Dimension.width + j) * 3;

        glm::vec3 p;
        p.r = m_Data[startIndex + 2];
        p.g = m_Data[startIndex + 1];
        p.b = m_Data[startIndex];
        return p;
    }

    /// Float data, R format, 1 channel
    template <>
    template <>
    float Image<float, ImageFormat::eR>::Get(size_t i, size_t j) const {
        if (!Inside(i, j)) {
            throw std::out_of_range("requested index is out of range");
        }

        return m_Data[i * m_Dimension.width + j];
    }

    /// Byte data, RGBA format, 4 channels
    template <>
    template <>
    glm::u8vec4 Image<uint8_t, ImageFormat::eRGBA>::Get(size_t i, size_t j) const {
        if (!Inside(i, j)) {
            throw std::out_of_range("requested index is out of range");
        }

        size_t startIndex = (i * m_Dimension.width + j) * 4;

        glm::u8vec4 p;
        p.r = m_Data[startIndex];
        p.g = m_Data[startIndex + 1];
        p.b = m_Data[startIndex + 2];
        p.a = m_Data[startIndex + 3];
        return p;
    }

    /// Byte data, BGRA format, 4 channels
    template <>
    template <>
    glm::u8vec4 Image<uint8_t, ImageFormat::eBGRA>::Get(size_t i, size_t j) const {
        if (!Inside(i, j)) {
            throw std::out_of_range("requested index is out of range");
        }

        size_t startIndex = (i * m_Dimension.width + j) * 4;

        glm::u8vec4 p;
        p.r = m_Data[startIndex + 2];
        p.g = m_Data[startIndex + 1];
        p.b = m_Data[startIndex];
        p.a = m_Data[startIndex + 3];
        return p;
    }

    /// Byte data, RGB format, 3 channels
    template <>
    template <>
    glm::u8vec3 Image<uint8_t, ImageFormat::eRGB>::Get(size_t i, size_t j) const {
        if (!Inside(i, j)) {
            throw std::out_of_range("requested index is out of range");
        }

        size_t startIndex = (i * m_Dimension.width + j) * 3;

        glm::u8vec3 p;
        p.r = m_Data[startIndex];
        p.g = m_Data[startIndex + 1];
        p.b = m_Data[startIndex + 2];
        return p;
    }

    /// Byte data, BGR format, 3 channels
    template <>
    template <>
    glm::u8vec3 Image<uint8_t, ImageFormat::eBGR>::Get(size_t i, size_t j) const {
        if (!Inside(i, j)) {
            throw std::out_of_range("requested index is out of range");
        }

        size_t startIndex = (i * m_Dimension.width + j) * 3;

        glm::u8vec3 p;
        p.r = m_Data[startIndex + 2];
        p.g = m_Data[startIndex + 1];
        p.b = m_Data[startIndex];
        return p;
    }

    /// Byte data, R format, 1 channel
    template <>
    template <>
    uint8_t Image<uint8_t, ImageFormat::eR>::Get(size_t i, size_t j) const {
        if (!Inside(i, j)) {
            throw std::out_of_range("requested index is out of range");
        }

        return m_Data[i * m_Dimension.width + j];
    }

    /// UInt data, RGBA format, 4 channels
    template <>
    template <>
    glm::u32vec4 Image<uint32_t, ImageFormat::eRGBA>::Get(size_t i, size_t j) const {
        if (!Inside(i, j)) {
            throw std::out_of_range("requested index is out of range");
        }

        size_t startIndex = (i * m_Dimension.width + j) * 4;

        glm::u32vec4 p;
        p.r = m_Data[startIndex];
        p.g = m_Data[startIndex + 1];
        p.b = m_Data[startIndex + 2];
        p.a = m_Data[startIndex + 3];
        return p;
    }

    /// Uint data, BGRA format, 4 channels
    template <>
    template <>
    glm::u32vec4 Image<uint32_t, ImageFormat::eBGRA>::Get(size_t i, size_t j) const {
        if (!Inside(i, j)) {
            throw std::out_of_range("requested index is out of range");
        }

        size_t startIndex = (i * m_Dimension.width + j) * 4;

        glm::u32vec4 p;
        p.r = m_Data[startIndex + 2];
        p.g = m_Data[startIndex + 1];
        p.b = m_Data[startIndex];
        p.a = m_Data[startIndex + 3];
        return p;
    }

    /// Uint data, RGB format, 3 channels
    template <>
    template <>
    glm::u32vec3 Image<uint32_t, ImageFormat::eRGB>::Get(size_t i, size_t j) const {
        if (!Inside(i, j)) {
            throw std::out_of_range("requested index is out of range");
        }

        size_t startIndex = (i * m_Dimension.width + j) * 3;

        glm::u32vec3 p;
        p.r = m_Data[startIndex];
        p.g = m_Data[startIndex + 1];
        p.b = m_Data[startIndex + 2];
        return p;
    }

    /// Uint data, BGR format, 3 channels
    template <>
    template <>
    glm::u32vec3 Image<uint32_t, ImageFormat::eBGR>::Get(size_t i, size_t j) const {
        if (!Inside(i, j)) {
            throw std::out_of_range("requested index is out of range");
        }

        size_t startIndex = (i * m_Dimension.width + j) * 3;

        glm::u32vec3 p;
        p.r = m_Data[startIndex + 2];
        p.g = m_Data[startIndex + 1];
        p.b = m_Data[startIndex];
        return p;
    }

    /// Uint data, R format, 1 channel
    template <>
    template <>
    uint32_t Image<uint32_t, ImageFormat::eR>::Get(size_t i, size_t j) const {
        if (!Inside(i, j)) {
            throw std::out_of_range("requested index is out of range");
        }

        return m_Data[i * m_Dimension.width + j];
    }

    /// Uint image, 1 channel
    template<>
    template<>
    uint32_t Image<uint32_t, ImageFormat::eR>::Get<uint32_t, math::Interpolation::eNearestNeighbour>(const glm::vec2& uv) const {
        glm::vec2 uvCoords = uv;

        if (uvCoords.x < 0.0f || uvCoords.x > 1.0f || uvCoords.y < 0.0f || uvCoords.y > 1.0f) {
            vd::Logger::warn("UV coords exceed image, down sampling to accepted values");

            uvCoords.x -= glm::floor(uvCoords.x);
            uvCoords.y -= glm::floor(uvCoords.y);
        }

        float uv_x = uvCoords.x * float(m_Dimension.width - 1);
        float uv_y = uvCoords.y * float(m_Dimension.height - 1);

        int uv_x_0 = int(std::floor(uv_x));
        int uv_y_0 = int(std::floor(uv_y));

        if ((std::abs(uv_x - std::floor(uv_x)) < std::numeric_limits<float>::epsilon() &&
             std::abs(uv_y - std::floor(uv_y)) < std::numeric_limits<float>::epsilon())) {
            return Get<uint32_t>(uv_x_0, uv_y_0);
        }

        int uv_x_1 = uv_x_0 + 1;
        int uv_y_1 = uv_y_0 + 1;

        math::Matrix2x2<uint32_t> m;
        m[0][0] = Get<uint32_t>(uv_x_0, uv_y_0);
        m[0][1] = Inside(uv_x_0, uv_y_1) ? Get<uint32_t>(uv_x_0, uv_y_1) : 0;
        m[1][0] = Inside(uv_x_1, uv_y_0) ? Get<uint32_t>(uv_x_1, uv_y_0) : 0;
        m[1][1] = Inside(uv_x_1, uv_y_1) ? Get<uint32_t>(uv_x_1, uv_y_1) : 0;

        float frac_x = uv_x - float(uv_x_0);
        float frac_y = uv_y - float(uv_y_0);

        return math::Interpolator<uint32_t>::NearestNeighbour(m, frac_x, frac_y);
    }

    /// Float image, 1 channel, bilinear
    template<>
    template<>
    float Image<float, ImageFormat::eR>::Get<float, math::Interpolation::eBilinear>(const glm::vec2& uv) const {
        glm::vec2 uvCoords = uv;

        if (uvCoords.x < 0.0f || uvCoords.x > 1.0f || uvCoords.y < 0.0f || uvCoords.y > 1.0f) {
            vd::Logger::warn("UV coords exceed image, down sampling to accepted values");

            uvCoords.x -= glm::floor(uvCoords.x);
            uvCoords.y -= glm::floor(uvCoords.y);
        }

        float uv_x = uvCoords.x * float(m_Dimension.width - 1);
        float uv_y = uvCoords.y * float(m_Dimension.height - 1);

        int uv_x_0 = int(std::floor(uv_x));
        int uv_y_0 = int(std::floor(uv_y));

        if ((std::abs(uv_x - std::floor(uv_x)) < std::numeric_limits<float>::epsilon() &&
             std::abs(uv_y - std::floor(uv_y)) < std::numeric_limits<float>::epsilon())) {
            return Get<float>(uv_x_0, uv_y_0);
        }

        int uv_x_1 = uv_x_0 + 1;
        int uv_y_1 = uv_y_0 + 1;

        math::Matrix2x2<float> m;
        m[0][0] = Get<float>(uv_x_0, uv_y_0);
        m[0][1] = Inside(uv_x_0, uv_y_1) ? Get<float>(uv_x_0, uv_y_1) : 0.0f;
        m[1][0] = Inside(uv_x_1, uv_y_0) ? Get<float>(uv_x_1, uv_y_0) : 0.0f;
        m[1][1] = Inside(uv_x_1, uv_y_1) ? Get<float>(uv_x_1, uv_y_1) : 0.0f;

        float frac_x = uv_x - float(uv_x_0);
        float frac_y = uv_y - float(uv_y_0);

        return math::Interpolator<float>::Bilinear(m, frac_x, frac_y);
    }

    /// Float image, 1 channel, bicubic
    template<>
    template<>
    float Image<float, ImageFormat::eR>::Get<float, math::Interpolation::eBicubic>(const glm::vec2& uv) const {
        glm::vec2 uvCoords = uv;

        if (uvCoords.x < 0.0f || uvCoords.x > 1.0f || uvCoords.y < 0.0f || uvCoords.y > 1.0f) {
            vd::Logger::warn("UV coords exceed image, down sampling to accepted values");

            uvCoords.x -= glm::floor(uvCoords.x);
            uvCoords.y -= glm::floor(uvCoords.y);
        }

        float uv_x = uvCoords.x * float(m_Dimension.width - 1);
        float uv_y = uvCoords.y * float(m_Dimension.height - 1);

        int uv_x_0 = int(std::floor(uv_x));
        int uv_y_0 = int(std::floor(uv_y));

        if ((std::abs(uv_x - std::floor(uv_x)) < std::numeric_limits<float>::epsilon() &&
             std::abs(uv_y - std::floor(uv_y)) < std::numeric_limits<float>::epsilon())) {
            return Get<float>(uv_x_0, uv_y_0);
        }

        int uv_x_1 = uv_x_0 + 1;
        int uv_y_1 = uv_y_0 + 1;
        int uv_x_m1 = uv_x_0 - 1;
        int uv_x_p1 = uv_x_1 + 1;
        int uv_y_m1 = uv_y_0 - 1;
        int uv_y_p1 = uv_y_1 + 1;

        math::Matrix4x4<float> m;
        m[0][0] = Inside(uv_x_m1, uv_y_m1) ? Get<float>(uv_x_m1, uv_y_m1) : 0.0f;
        m[0][1] = Inside(uv_x_m1, uv_y_0) ? Get<float>(uv_x_m1, uv_y_0) : 0.0f;
        m[0][2] = Inside(uv_x_m1, uv_y_1) ? Get<float>(uv_x_m1, uv_y_1) : 0.0f;
        m[0][3] = Inside(uv_x_m1, uv_y_p1) ? Get<float>(uv_x_m1, uv_y_p1) : 0.0f;

        m[1][0] = Inside(uv_x_0, uv_y_m1) ? Get<float>(uv_x_0, uv_y_m1) : 0.0f;
        m[1][1] = Get<float>(uv_x_0, uv_y_0);
        m[1][2] = Inside(uv_x_0, uv_y_1) ? Get<float>(uv_x_0, uv_y_1) : 0.0f;
        m[1][3] = Inside(uv_x_0, uv_y_p1) ? Get<float>(uv_x_0, uv_y_p1) : 0.0f;

        m[2][0] = Inside(uv_x_1, uv_y_m1) ? Get<float>(uv_x_1, uv_y_m1) : 0.0f;
        m[2][1] = Inside(uv_x_1, uv_y_0) ? Get<float>(uv_x_1, uv_y_0) : 0.0f;
        m[2][2] = Inside(uv_x_1, uv_y_1) ? Get<float>(uv_x_1, uv_y_1) : 0.0f;
        m[2][3] = Inside(uv_x_1, uv_y_p1) ? Get<float>(uv_x_1, uv_y_p1) : 0.0f;

        m[3][0] = Inside(uv_x_p1, uv_y_m1) ? Get<float>(uv_x_p1, uv_y_m1) : 0.0f;
        m[3][1] = Inside(uv_x_p1, uv_y_0) ? Get<float>(uv_x_p1, uv_y_0) : 0.0f;
        m[3][2] = Inside(uv_x_p1, uv_y_1) ? Get<float>(uv_x_p1, uv_y_1) : 0.0f;
        m[3][3] = Inside(uv_x_p1, uv_y_p1) ? Get<float>(uv_x_p1, uv_y_p1) : 0.0f;

        float frac_x = uv_x - float(uv_x_0);
        float frac_y = uv_y - float(uv_y_0);

        return math::Interpolator<float>::Bicubic(m, frac_x, frac_y);
    }

    template<typename T, ImageFormat F>
    std::string& Image<T, F>::Name() {
        return m_Name;
    }

    template<typename T, ImageFormat F>
    vd::Dimension& Image<T, F>::Dimension() {
        return m_Dimension;
    }

    template<typename T, ImageFormat F>
    std::vector<T>& Image<T, F>::Data() {
        return m_Data;
    }

    template<typename T, ImageFormat F>
    uint64_t Image<T, F>::Width() const {
        return m_Dimension.width;
    }

    template<typename T, ImageFormat F>
    uint64_t Image<T, F>::Height() const {
        return m_Dimension.height;
    }

    template<typename T, ImageFormat F>
    bool Image<T, F>::Inside(int i, int j) const {
        return !(i < 0 || j < 0 || j >= m_Dimension.width || i >= m_Dimension.height);
    }
}
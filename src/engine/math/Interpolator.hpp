//
// Created by Vali on 11/13/2020.
//

#ifndef VD_GAME_ENGINE_INTERPOLATOR_HPP
#define VD_GAME_ENGINE_INTERPOLATOR_HPP

#include <glm/glm.hpp>

#include <array>
#include <stdexcept>

namespace vd::math {
    enum Interpolation {
        eNearestNeighbour = 0,
        eLinear,
        eBilinear,
        eCubic,
        eBicubic
    };

    template <typename F>
    using Matrix2x2 = std::array<std::array<F, 2>, 2>;

    template <typename F>
    using Vector4 = std::array<F, 4>;

    template <typename F>
    using Matrix4x4 = std::array<std::array<F, 4>, 4>;

    template <class T>
    class Interpolator {
    public:

        static T NearestNeighbour(const T& l, const T& r, float x);
        static T NearestNeighbour(const Matrix2x2<T>& p, float x, float y);
        static T Linear(const T& l, const T& r, float x);
        static T Bilinear(const Matrix2x2<T>& p, float x, float y);
        static T Cubic(const Vector4<T>& p, float x);
        static T Bicubic(const Matrix4x4<T>& p, float x, float y);
    };

}

#endif //VD_GAME_ENGINE_INTERPOLATOR_HPP

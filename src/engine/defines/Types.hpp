//
// Created by Vali on 11/12/2020.
//

#ifndef VD_GAME_ENGINE_TYPES_HPP
#define VD_GAME_ENGINE_TYPES_HPP

#include <cstdint>
#include <functional>

namespace vd {
    namespace internal {
        template<typename T>
        struct NumericDimension {
            T width;
            T height;

            NumericDimension() : width(T(0)), height(T(0)) {}
            NumericDimension(T width, T height) : width(width), height(height) {}
        };
    }

    typedef internal::NumericDimension<uint64_t> Dimension;
    typedef internal::NumericDimension<float>    FloatDimension;

    template <typename T> using Getter = std::function<T()>;
    template <typename T> using Setter = std::function<void(T&)>;
    template <typename T> using User = std::function<void(const T&)>;

    typedef Getter<void>   Consumer;
    typedef Getter<bool>   Predicate;

    inline Consumer     g_kEmptyConsumer    = []() { };
    inline Predicate    g_kEmptyPredicate   = []() { return true; };

    typedef float   Timestep;
}

#endif //VD_GAME_ENGINE_TYPES_HPP

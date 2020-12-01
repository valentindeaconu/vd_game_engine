//
// Created by Vali on 11/13/2020.
//

#include "Interpolator.hpp"

namespace vd::math {
    /// Forward decl
    template uint32_t Interpolator<uint32_t>::NearestNeighbour(const uint32_t &l, const uint32_t &r, float x);
    template float Interpolator<float>::NearestNeighbour(const float &l, const float &r, float x);

    /// Impl
    template<typename T>
    T Interpolator<T>::NearestNeighbour(const T& l, const T& r, float x) {
        return x < 0.5f ? l : r;
    }

    template<>
    glm::vec2 Interpolator<glm::vec2>::NearestNeighbour(const glm::vec2& l, const glm::vec2& r, float x) {
        return glm::vec2(
            Interpolator<float>::NearestNeighbour(l.x, r.x, x),
            Interpolator<float>::NearestNeighbour(l.y, r.y, x)
        );
    }

    template<>
    glm::u32vec2 Interpolator<glm::u32vec2>::NearestNeighbour(const glm::u32vec2& l, const glm::u32vec2& r, float x) {
        return glm::u32vec2(
            Interpolator<uint32_t>::NearestNeighbour(l.x, r.x, x),
            Interpolator<uint32_t>::NearestNeighbour(l.y, r.y, x)
        );
    }

    template<>
    glm::vec3 Interpolator<glm::vec3>::NearestNeighbour(const glm::vec3& l, const glm::vec3& r, float x) {
        return glm::vec3(
            Interpolator<float>::NearestNeighbour(l.x, r.x, x),
            Interpolator<float>::NearestNeighbour(l.y, r.y, x),
            Interpolator<float>::NearestNeighbour(l.z, r.z, x)
        );
    }

    template<>
    glm::u32vec3 Interpolator<glm::u32vec3>::NearestNeighbour(const glm::u32vec3& l, const glm::u32vec3& r, float x) {
        return glm::u32vec3(
            Interpolator<uint32_t>::NearestNeighbour(l.x, r.x, x),
            Interpolator<uint32_t>::NearestNeighbour(l.y, r.y, x),
            Interpolator<uint32_t>::NearestNeighbour(l.z, r.z, x)
        );
    }

    template<>
    glm::vec4 Interpolator<glm::vec4>::NearestNeighbour(const glm::vec4& l, const glm::vec4& r, float x) {
        return glm::vec4(
            Interpolator<float>::NearestNeighbour(l.x, r.x, x),
            Interpolator<float>::NearestNeighbour(l.y, r.y, x),
            Interpolator<float>::NearestNeighbour(l.z, r.z, x),
            Interpolator<float>::NearestNeighbour(l.w, r.w, x)
        );
    }

    template<>
    glm::u32vec4 Interpolator<glm::u32vec4>::NearestNeighbour(const glm::u32vec4& l, const glm::u32vec4& r, float x) {
        return glm::u32vec4(
            Interpolator<uint32_t>::NearestNeighbour(l.x, r.x, x),
            Interpolator<uint32_t>::NearestNeighbour(l.y, r.y, x),
            Interpolator<uint32_t>::NearestNeighbour(l.z, r.z, x),
            Interpolator<uint32_t>::NearestNeighbour(l.w, r.w, x)
        );
    }

    /// Forward decl
    template uint32_t Interpolator<uint32_t>::NearestNeighbour(const Matrix2x2<uint32_t>& p, float x, float y);
    template float Interpolator<float>::NearestNeighbour(const Matrix2x2<float>& p, float x, float y);

    /// Impl
    template<typename T>
    T Interpolator<T>::NearestNeighbour(const Matrix2x2<T>& p, float x, float y) {
        T l = Interpolator<T>::NearestNeighbour(p[0][0], p[0][1], x);
        T r = Interpolator<T>::NearestNeighbour(p[1][0], p[1][1], x);

        return Interpolator<T>::NearestNeighbour(l, r, y);
    }

    template<>
    glm::vec2 Interpolator<glm::vec2>::NearestNeighbour(const Matrix2x2<glm::vec2>& p, float x, float y) {
        glm::vec2 l = Interpolator<glm::vec2>::NearestNeighbour(p[0][0], p[0][1], x);
        glm::vec2 r = Interpolator<glm::vec2>::NearestNeighbour(p[1][0], p[1][1], x);

        return Interpolator<glm::vec2>::NearestNeighbour(l, r, y);
    }

    template<>
    glm::u32vec2 Interpolator<glm::u32vec2>::NearestNeighbour(const Matrix2x2<glm::u32vec2>& p, float x, float y) {
        glm::u32vec2 l = Interpolator<glm::u32vec2>::NearestNeighbour(p[0][0], p[0][1], x);
        glm::u32vec2 r = Interpolator<glm::u32vec2>::NearestNeighbour(p[1][0], p[1][1], x);

        return Interpolator<glm::u32vec2>::NearestNeighbour(l, r, y);
    }

    template<>
    glm::vec3 Interpolator<glm::vec3>::NearestNeighbour(const Matrix2x2<glm::vec3>& p, float x, float y) {
        glm::vec3 l = Interpolator<glm::vec3>::NearestNeighbour(p[0][0], p[0][1], x);
        glm::vec3 r = Interpolator<glm::vec3>::NearestNeighbour(p[1][0], p[1][1], x);

        return Interpolator<glm::vec3>::NearestNeighbour(l, r, y);
    }

    template<>
    glm::u32vec3 Interpolator<glm::u32vec3>::NearestNeighbour(const Matrix2x2<glm::u32vec3>& p, float x, float y) {
        glm::u32vec3 l = Interpolator<glm::u32vec3>::NearestNeighbour(p[0][0], p[0][1], x);
        glm::u32vec3 r = Interpolator<glm::u32vec3>::NearestNeighbour(p[1][0], p[1][1], x);

        return Interpolator<glm::u32vec3>::NearestNeighbour(l, r, y);
    }

    template<>
    glm::vec4 Interpolator<glm::vec4>::NearestNeighbour(const Matrix2x2<glm::vec4>& p, float x, float y) {
        glm::vec4 l = Interpolator<glm::vec4>::NearestNeighbour(p[0][0], p[0][1], x);
        glm::vec4 r = Interpolator<glm::vec4>::NearestNeighbour(p[1][0], p[1][1], x);

        return Interpolator<glm::vec4>::NearestNeighbour(l, r, y);
    }

    template<>
    glm::u32vec4 Interpolator<glm::u32vec4>::NearestNeighbour(const Matrix2x2<glm::u32vec4>& p, float x, float y) {
        glm::u32vec4 l = Interpolator<glm::u32vec4>::NearestNeighbour(p[0][0], p[0][1], x);
        glm::u32vec4 r = Interpolator<glm::u32vec4>::NearestNeighbour(p[1][0], p[1][1], x);

        return Interpolator<glm::u32vec4>::NearestNeighbour(l, r, y);
    }

    template<>
    float Interpolator<float>::Linear(const float& l, const float& r, float x) {
        return glm::mix(l, r, x);
    }

    template<>
    glm::vec2 Interpolator<glm::vec2>::Linear(const glm::vec2& l, const glm::vec2& r, float x) {
        return glm::vec2(
            Interpolator<float>::Linear(l.x, r.x, x),
            Interpolator<float>::Linear(l.y, r.y, x)
        );
    }

    template<>
    glm::vec3 Interpolator<glm::vec3>::Linear(const glm::vec3& l, const glm::vec3& r, float x) {
        return glm::vec3(
            Interpolator<float>::Linear(l.x, r.x, x),
            Interpolator<float>::Linear(l.y, r.y, x),
            Interpolator<float>::Linear(l.z, r.z, x)
        );
    }

    template<>
    glm::vec4 Interpolator<glm::vec4>::Linear(const glm::vec4& l, const glm::vec4& r, float x) {
        return glm::vec4(
            Interpolator<float>::Linear(l.x, r.x, x),
            Interpolator<float>::Linear(l.y, r.y, x),
            Interpolator<float>::Linear(l.z, r.z, x),
            Interpolator<float>::Linear(l.w, r.w, x)
        );
    }

    template<>
    float Interpolator<float>::Bilinear(const Matrix2x2<float>& p, float x, float y) {
        float l = Interpolator<float>::Linear(p[0][0], p[0][1], x);
        float r = Interpolator<float>::Linear(p[1][0], p[1][1], x);

        return Interpolator<float>::Linear(l, r, y);
    }

    template<>
    glm::vec2 Interpolator<glm::vec2>::Bilinear(const Matrix2x2<glm::vec2>& p, float x, float y) {
        glm::vec2 l = Interpolator<glm::vec2>::Linear(p[0][0], p[0][1], x);
        glm::vec2 r = Interpolator<glm::vec2>::Linear(p[1][0], p[1][1], x);

        return Interpolator<glm::vec2>::Linear(l, r, y);
    }

    template<>
    glm::vec3 Interpolator<glm::vec3>::Bilinear(const Matrix2x2<glm::vec3>& p, float x, float y) {
        glm::vec3 l = Interpolator<glm::vec3>::Linear(p[0][0], p[0][1], x);
        glm::vec3 r = Interpolator<glm::vec3>::Linear(p[1][0], p[1][1], x);

        return Interpolator<glm::vec3>::Linear(l, r, y);
    }

    template<>
    glm::vec4 Interpolator<glm::vec4>::Bilinear(const Matrix2x2<glm::vec4>& p, float x, float y) {
        glm::vec4 l = Interpolator<glm::vec4>::Linear(p[0][0], p[0][1], x);
        glm::vec4 r = Interpolator<glm::vec4>::Linear(p[1][0], p[1][1], x);

        return Interpolator<glm::vec4>::Linear(l, r, y);
    }

    template<>
    float Interpolator<float>::Cubic(const Vector4<float>& p, float x) {
        return p[1] + 0.5f * x * (p[2] - p[0] + x * (2.0f * p[0] - 5.0f* p[1] + 4.0f * p[2] - p[3] + x * (3.0f * (p[1] - p[2]) + p[3] - p[0])));
    }

    template<>
    glm::vec2 Interpolator<glm::vec2>::Cubic(const Vector4<glm::vec2>& p, float x) {
        std::array<float, 4> px = { p[0].x, p[1].x, p[2].x, p[3].x };
        std::array<float, 4> py = { p[0].y, p[1].y, p[2].y, p[3].y };

        return glm::vec2(
            Interpolator<float>::Cubic(px, x),
            Interpolator<float>::Cubic(py, x)
        );
    }

    template<>
    glm::vec3 Interpolator<glm::vec3>::Cubic(const Vector4<glm::vec3>& p, float x) {
        std::array<float, 4> px = { p[0].x, p[1].x, p[2].x, p[3].x };
        std::array<float, 4> py = { p[0].y, p[1].y, p[2].y, p[3].y };
        std::array<float, 4> pz = { p[0].z, p[1].z, p[2].z, p[3].z };

        return glm::vec3(
            Interpolator<float>::Cubic(px, x),
            Interpolator<float>::Cubic(py, x),
            Interpolator<float>::Cubic(pz, x)
        );
    }

    template<>
    glm::vec4 Interpolator<glm::vec4>::Cubic(const Vector4<glm::vec4>& p, float x) {
        std::array<float, 4> px = { p[0].x, p[1].x, p[2].x, p[3].x };
        std::array<float, 4> py = { p[0].y, p[1].y, p[2].y, p[3].y };
        std::array<float, 4> pz = { p[0].z, p[1].z, p[2].z, p[3].z };
        std::array<float, 4> pw = { p[0].w, p[1].w, p[2].w, p[3].w };

        return glm::vec4(
            Interpolator<float>::Cubic(px, x),
            Interpolator<float>::Cubic(py, x),
            Interpolator<float>::Cubic(pz, x),
            Interpolator<float>::Cubic(pw, x)
        );
    }

    template<>
    float Interpolator<float>::Bicubic(const Matrix4x4<float>& p, float x, float y) {
        Vector4<float> r{};
        r[0] = Interpolator<float>::Cubic(p[0], x);
        r[1] = Interpolator<float>::Cubic(p[1], x);
        r[2] = Interpolator<float>::Cubic(p[2], x);
        r[2] = Interpolator<float>::Cubic(p[3], x);

        return Interpolator<float>::Cubic(r, y);;
    }

    template<>
    glm::vec2 Interpolator<glm::vec2>::Bicubic(const Matrix4x4<glm::vec2>& p, float x, float y) {
        Vector4<glm::vec2> r{};
        r[0] = Interpolator<glm::vec2>::Cubic(p[0], x);
        r[1] = Interpolator<glm::vec2>::Cubic(p[1], x);
        r[2] = Interpolator<glm::vec2>::Cubic(p[2], x);
        r[2] = Interpolator<glm::vec2>::Cubic(p[3], x);

        return Interpolator<glm::vec2>::Cubic(r, y);;
    }

    template<>
    glm::vec3 Interpolator<glm::vec3>::Bicubic(const Matrix4x4<glm::vec3>& p, float x, float y) {
        Vector4<glm::vec3> r{};
        r[0] = Interpolator<glm::vec3>::Cubic(p[0], x);
        r[1] = Interpolator<glm::vec3>::Cubic(p[1], x);
        r[2] = Interpolator<glm::vec3>::Cubic(p[2], x);
        r[2] = Interpolator<glm::vec3>::Cubic(p[3], x);

        return Interpolator<glm::vec3>::Cubic(r, y);;
    }

    template<>
    glm::vec4 Interpolator<glm::vec4>::Bicubic(const Matrix4x4<glm::vec4>& p, float x, float y) {
        Vector4<glm::vec4> r{};
        r[0] = Interpolator<glm::vec4>::Cubic(p[0], x);
        r[1] = Interpolator<glm::vec4>::Cubic(p[1], x);
        r[2] = Interpolator<glm::vec4>::Cubic(p[2], x);
        r[2] = Interpolator<glm::vec4>::Cubic(p[3], x);

        return Interpolator<glm::vec4>::Cubic(r, y);;
    }
}
//
// Created by Vali on 8/26/2020.
//

#ifndef VD_GAME_ENGINE_SHADOWBOX_HPP
#define VD_GAME_ENGINE_SHADOWBOX_HPP

#include <engine/light/Light.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/window/Window.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <memory>
#include <algorithm>

namespace mod::shadow {
    class ShadowBox {
    public:
        struct Coordinates {
            float min;
            float max;
        };

        ShadowBox(std::shared_ptr<glm::mat4> viewMatrixPtr, float distance, float offset);
        ~ShadowBox();

        void Link();
        void Update();

        [[nodiscard]] glm::vec3 Center() const;

        [[nodiscard]] const Coordinates& X() const;
        [[nodiscard]] const Coordinates& Y() const;
        [[nodiscard]] const Coordinates& Z() const;
    private:
        std::vector<glm::vec4> FrustumVertices();
        void UpdateWidthsAndHeights();

        Coordinates m_X, m_Y, m_Z;

        const float m_kDistance;
        const float m_kOffset;

        std::shared_ptr<glm::mat4> m_ViewMatrixPtr;

        vd::FloatDimension m_Far;
        vd::FloatDimension m_Near;

        typedef std::function<float()>      ValueGetter;
        typedef std::function<glm::vec3()>  VectorGetter;

        ValueGetter m_AspectRatioGetter;
        ValueGetter m_NearPlaneGetter;
        ValueGetter m_FieldOfViewGetter;

        vd::Predicate m_PerspectiveChangedChecker;

        VectorGetter m_CameraPositionGetter;
        VectorGetter m_CameraForwardGetter;
        VectorGetter m_CameraRightGetter;
        VectorGetter m_CameraUpGetter;
    };
    typedef std::shared_ptr<ShadowBox>  ShadowBoxPtr;
}


#endif //VD_GAME_ENGINE_SHADOWBOX_HPP

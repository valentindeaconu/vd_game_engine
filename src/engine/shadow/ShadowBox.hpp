//
// Created by Vali on 8/26/2020.
//

#ifndef VD_GAME_ENGINE_SHADOWBOX_HPP
#define VD_GAME_ENGINE_SHADOWBOX_HPP

#include <engine/model/Light.hpp>
#include <engine/core/Camera.hpp>
#include <engine/core/Window.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <memory>
#include <algorithm>

namespace vd::shadow
{
    class ShadowBox
    {
    public:
        struct Coordinates
        {
            float min;
            float max;
        };

        ShadowBox(const core::WindowPtr& windowPtr,
                  const core::CameraPtr& cameraPtr,
                  const std::shared_ptr<glm::mat4>& lightViewPtr,
                  float distance,
                  float offset);
        ~ShadowBox();

        void update(const model::LightPtr& sunPtr);

        [[nodiscard]] glm::vec3 getCenter() const;

        [[nodiscard]] const Coordinates& getX() const;
        [[nodiscard]] const Coordinates& getY() const;
        [[nodiscard]] const Coordinates& getZ() const;
    private:
        std::vector<glm::vec4> computeFrustumVertices();
        void updateWidthsAndHeights();

        Coordinates X, Y, Z;

        const float kDistance;
        const float kOffset;

        core::WindowPtr windowPtr;
        core::CameraPtr cameraPtr;

        std::shared_ptr<glm::mat4> lightViewPtr;

        float far_width;
        float near_width;
        float far_height;
        float near_height;
    };
    typedef std::shared_ptr<ShadowBox>  ShadowBoxPtr;
}


#endif //VD_GAME_ENGINE_SHADOWBOX_HPP

//
// Created by Vali on 8/26/2020.
//

#include "ShadowBox.hpp"

namespace vd::shadow
{
    ShadowBox::ShadowBox(const core::WindowPtr& windowPtr,
                         const core::CameraPtr& cameraPtr,
                         const std::shared_ptr<glm::mat4>& lightViewPtr,
                         float distance,
                         float offset)
        : windowPtr(windowPtr)
        , cameraPtr(cameraPtr)
        , lightViewPtr(lightViewPtr)
        , kDistance(distance)
        , kOffset(offset)
    {
        updateWidthsAndHeights();
    }

    ShadowBox::~ShadowBox() = default;

    void ShadowBox::update(const model::LightPtr& sunPtr) {
        std::vector<glm::vec4> points = computeFrustumVertices();

        X.min = X.max = points.front().x;
        Y.min = Y.max = points.front().y;
        Z.min = Z.max = points.front().z;

        for (int i = 1; i < points.size(); ++i) {
            X.max = std::max(X.max, points[i].x);
            X.min = std::min(X.min, points[i].x);

            Y.max = std::max(Y.max, points[i].y);
            Y.min = std::min(Y.min, points[i].y);

            Z.max = std::max(Z.max, points[i].z);
            Z.min = std::min(Z.min, points[i].z);
        }

        Z.max += kOffset;
    }

    glm::vec3 ShadowBox::getCenter() const
    {
        glm::vec4 center;

        center.x = (X.min + X.max) / 2.0f;
        center.y = (Y.min + Y.max) / 2.0f;
        center.z = (Z.min + Z.max) / 2.0f;
        center.w = 1.0f;

        // center coordinates are in light space, so convert them back to world space by multiplying with
        // inverse of lightView matrix
        return glm::vec3(glm::inverse(*lightViewPtr) * center);
    }

    const ShadowBox::Coordinates& ShadowBox::getX() const {
        return X;
    }

    const ShadowBox::Coordinates& ShadowBox::getY() const {
        return Y;
    }

    const ShadowBox::Coordinates& ShadowBox::getZ() const {
        return Z;
    }

    std::vector<glm::vec4> ShadowBox::computeFrustumVertices() {
        glm::vec3 cameraPosition = cameraPtr->getPosition();
        glm::vec3 forward = cameraPtr->getDirection();
        glm::vec3 right = cameraPtr->getRightDirection();
        glm::vec3 up = cameraPtr->getUpDirection();

        float zNear = windowPtr->getNearPlane();

        // Compute center points
        glm::vec3 fc = cameraPosition + (forward * kDistance); // far plane center point
        glm::vec3 nc = cameraPosition + (forward * zNear); // near plane center point

        if (windowPtr->isPerspectiveChanged()) {
            updateWidthsAndHeights();
        }

        return std::vector<glm::vec4>({
            *lightViewPtr * glm::vec4(nc - up*near_height - right*near_width, 1.0f), // near bottom left
            *lightViewPtr * glm::vec4(nc + up*near_height - right*near_width, 1.0f), // near top left
            *lightViewPtr * glm::vec4(nc + up*near_height + right*near_width, 1.0f), // near top right
            *lightViewPtr * glm::vec4(nc - up*near_height + right*near_width, 1.0f), // near bottom right

            *lightViewPtr * glm::vec4(fc - up*far_height - right*far_width, 1.0f), // far bottom left
            *lightViewPtr * glm::vec4(fc + up*far_height - right*far_width, 1.0f), // far top left
            *lightViewPtr * glm::vec4(fc + up*far_height + right*far_width, 1.0f), // far top right
            *lightViewPtr * glm::vec4(fc - up*far_height + right*far_width, 1.0f) // far bottom right
        });
    }

    void ShadowBox::updateWidthsAndHeights() {
        // these heights and widths are half the heights and widths of
        // the near and far plane rectangles

        float zNear = windowPtr->getNearPlane();
        float fovY = windowPtr->getFieldOfView();
        float aspect = windowPtr->getAspectRatio();

        far_width = kDistance * std::tan(glm::radians(fovY));
        near_width = zNear * std::tan(glm::radians(fovY));
        far_height = far_width / aspect;
        near_height = near_width / aspect;

        /*float near_height = glm::tan(fovY / 2.0f) * zNear;
        float near_width = near_height * aspect;
        float far_height = glm::tan(fovY / 2.0f) * zFar * kDistance;
        float far_width = far_height * aspect;*/
    }
}



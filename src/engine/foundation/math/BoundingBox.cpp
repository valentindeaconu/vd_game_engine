//
// Created by Vali on 9/19/2020.
//

#include "BoundingBox.hpp"

namespace vd::math {
    BoundingBox::BoundingBox()
        : bottomLeft(0.0f)
        , topRight(0.0f)
    {
    }

    BoundingBox::BoundingBox(const glm::vec3& bottomLeft, const glm::vec3& topRight)
        : bottomLeft(bottomLeft)
        , topRight(topRight)
    {
    }

    BoundingBox::BoundingBox(const model::MeshPtr& meshPtr)
        : bottomLeft(0.0f)
        , topRight(0.0f)
    {
        wrapMesh(meshPtr);
    }

    BoundingBox::~BoundingBox() = default;

    void BoundingBox::wrapMesh(const model::MeshPtr& meshPtr) {
        if (meshPtr != nullptr && !meshPtr->vertices.empty()) {
            bottomLeft.x = topRight.x = meshPtr->vertices.front().Position.x;
            bottomLeft.y = topRight.y = meshPtr->vertices.front().Position.y;
            bottomLeft.z = topRight.z = meshPtr->vertices.front().Position.z;

            for (size_t i = 1; i < meshPtr->vertices.size(); ++i) {
                model::Vertex &v = meshPtr->vertices[i];

                topRight.x = std::max(topRight.x, v.Position.x);
                bottomLeft.x = std::min(bottomLeft.x, v.Position.x);

                topRight.y = std::max(topRight.y, v.Position.y);
                bottomLeft.y = std::min(bottomLeft.y, v.Position.y);

                topRight.z = std::max(topRight.z, v.Position.z);
                bottomLeft.z = std::min(bottomLeft.z, v.Position.z);
            }
        }
    }

    bool BoundingBox::isEmpty() const {
        return bottomLeft == glm::vec3(0.0f) && topRight == glm::vec3(0.0f);
    }

    const glm::vec3& BoundingBox::getBottomLeft() const {
        return bottomLeft;
    }

    const glm::vec3& BoundingBox::getTopRight() const {
        return topRight;
    }

    BoundingBox BoundingBox::withTransform(const Transform& transform) const {
        return BoundingBox(
            glm::vec3(transform.get() * glm::vec4(bottomLeft, 1.0f)),
            glm::vec3(transform.get() * glm::vec4(topRight, 1.0f))
        );
    }

}
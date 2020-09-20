//
// Created by Vali on 9/19/2020.
//

#ifndef VD_GAME_ENGINE_BOUNDINGBOX_HPP
#define VD_GAME_ENGINE_BOUNDINGBOX_HPP

#include <engine/model/Mesh.hpp>

#include <algorithm>

#include "Transform.hpp"

namespace vd::math {
    class BoundingBox {
    public:
        BoundingBox();
        BoundingBox(const glm::vec3& bottomLeft, const glm::vec3& topRight);

        [[noreturn]] explicit BoundingBox(const model::MeshPtr& meshPtr);

        ~BoundingBox();

        void wrapMesh(const model::MeshPtr& meshPtr);

        [[nodiscard]] bool isEmpty() const;

        [[nodiscard]] const glm::vec3& getBottomLeft() const;
        [[nodiscard]] const glm::vec3& getTopRight() const;

        [[nodiscard]] BoundingBox withTransform(const Transform& transform) const;
    private:
        glm::vec3 bottomLeft;
        glm::vec3 topRight;
    };
    typedef std::vector<BoundingBox>    BoundingBoxVec;
}


#endif //VD_GAME_ENGINE_BOUNDINGBOX_HPP

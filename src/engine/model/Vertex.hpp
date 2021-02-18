//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_VERTEX_HPP
#define VD_GAME_ENGINE_VERTEX_HPP

#include <glm/glm.hpp>
#include <memory>

#include <engine/exception/Exceptions.hpp>

namespace vd::model {

    struct Vertex2D {
        glm::vec2 Position;
        glm::vec2 TexCoords;

        Vertex2D(float x, float y);
        Vertex2D(float x, float y, float u, float v);
        explicit Vertex2D(const glm::vec2& position = glm::vec2(0.0f));
        Vertex2D(const glm::vec2& position, const glm::vec2& texCoords);
    };

    struct Vertex3D {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;

        Vertex3D(float x, float z);
        explicit Vertex3D(const glm::vec2& position);
        Vertex3D(float x, float y, float z);
        explicit Vertex3D(const glm::vec3& position = glm::vec3(0.0f));
        Vertex3D(const glm::vec3& position, const glm::vec3& normal);
        Vertex3D(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords);

        glm::vec2 xz();
    };

}

#endif //VD_GAME_ENGINE_VERTEX_HPP

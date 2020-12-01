//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_VERTEX_HPP
#define VD_GAME_ENGINE_VERTEX_HPP

#include <glm/glm.hpp>
#include <memory>

#include <engine/exception/Exceptions.hpp>

namespace vd::model {
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;

        Vertex(float x, float z);
        explicit Vertex(const glm::vec2& position);
        Vertex(float x, float y, float z);
        explicit Vertex(const glm::vec3& position = glm::vec3(0.0f));
        Vertex(const glm::vec3& position, const glm::vec3& normal);
        Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords);

        glm::vec2 xz();
    };

}

#endif //VD_GAME_ENGINE_VERTEX_HPP

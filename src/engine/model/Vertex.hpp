//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_VERTEX_HPP
#define VD_GAME_ENGINE_VERTEX_HPP

#include <glm/glm.hpp>

namespace vd::model {
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;

        Vertex() : Position(0.0f), Normal(0.0f), TexCoords(0.0f) { }
        explicit Vertex(const glm::vec3& position,
                        const glm::vec3& normal = glm::vec3(0.0f),
                        const glm::vec2& texCoords = glm::vec2(0.0f))
            : Position(position)
            , Normal(normal)
            , TexCoords(texCoords)
        { }
    };
}

#endif //VD_GAME_ENGINE_VERTEX_HPP

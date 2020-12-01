//
// Created by Vali on 12/1/2020.
//

#include "Vertex.hpp"

namespace vd::model {
    Vertex::Vertex(float x, float z)
        : Position(x, 0.0f, z)
        , Normal()
        , TexCoords()
    {
    }

    Vertex::Vertex(const glm::vec2& position)
        : Position(position.x, 0.0f, position.y)
        , Normal()
        , TexCoords()
    {
    }

    Vertex::Vertex(float x, float y, float z)
        : Position(x, y, z)
        , Normal()
        , TexCoords()
    {
    }

    Vertex::Vertex(const glm::vec3& position)
        : Position(position)
        , Normal()
        , TexCoords()
    {
    }

    Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal)
        : Position(position)
        , Normal(normal)
        , TexCoords()
    {
    }

    Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords)
        : Position(position)
        , Normal(normal)
        , TexCoords(texCoords)
    {
    }

    glm::vec2 Vertex::xz() {
        return glm::vec2(Position.x, Position.z);
    }

}
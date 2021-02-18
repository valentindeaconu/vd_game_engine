//
// Created by Vali on 12/1/2020.
//

#include "Vertex.hpp"

namespace vd::model {

    Vertex2D::Vertex2D(float x, float y)
        : Position(x, y)
        , TexCoords(0.0f)
    {
    }

    Vertex2D::Vertex2D(float x, float y, float u, float v)
        : Position(x, y)
        , TexCoords(u, v)
    {
    }

    Vertex2D::Vertex2D(const glm::vec2& position)
        : Position(position)
        , TexCoords(0.0f)
    {
    }

    Vertex2D::Vertex2D(const glm::vec2& position, const glm::vec2& texCoords)
        : Position(position)
        , TexCoords(texCoords)
    {
    }

    Vertex3D::Vertex3D(float x, float z)
        : Position(x, 0.0f, z)
        , Normal()
        , TexCoords()
    {
    }

    Vertex3D::Vertex3D(const glm::vec2& position)
        : Position(position.x, 0.0f, position.y)
        , Normal()
        , TexCoords()
    {
    }

    Vertex3D::Vertex3D(float x, float y, float z)
        : Position(x, y, z)
        , Normal()
        , TexCoords()
    {
    }

    Vertex3D::Vertex3D(const glm::vec3& position)
        : Position(position)
        , Normal()
        , TexCoords()
    {
    }

    Vertex3D::Vertex3D(const glm::vec3& position, const glm::vec3& normal)
        : Position(position)
        , Normal(normal)
        , TexCoords()
    {
    }

    Vertex3D::Vertex3D(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords)
        : Position(position)
        , Normal(normal)
        , TexCoords(texCoords)
    {
    }

    glm::vec2 Vertex3D::xz() {
        return glm::vec2(Position.x, Position.z);
    }

}
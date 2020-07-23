#ifndef __VERTEX_HPP_
#define __VERTEX_HPP_

#include <glm/glm.hpp>

namespace vd::model
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };
}

#endif // !__VERTEX_HPP_

#ifndef __MATERIAL_HPP_
#define __MATERIAL_HPP_

#include <glm/glm.hpp>

#include <string>

#include "Texture.hpp"

namespace vd::model
{
    struct Material
    {
        std::string name;
        Texture2DPtr diffuseMap;
        Texture2DPtr normalMap;
        Texture2DPtr displaceMap;
        Texture2DPtr ambientMap;
        Texture2DPtr specularMap;
        Texture2DPtr alphaMap;
        Texture2DPtr bumpMap;
        glm::vec4 color;
        glm::vec3 emission;
        float shininess;
        float displaceScale;
        float horizontalScale;
    };
    typedef std::vector<Material>   MaterialVec;
}

#endif // !__MATERIAL_HPP_

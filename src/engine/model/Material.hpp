//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_MATERIAL_HPP
#define VD_GAME_ENGINE_MATERIAL_HPP

#include <glm/glm.hpp>

#include <string>

#include <engine/api/gl/Texture.hpp>

namespace vd::model {
    struct Material {
        std::string name;
        gl::Texture2DPtr diffuseMap;
        gl::Texture2DPtr normalMap;
        gl::Texture2DPtr displaceMap;
        gl::Texture2DPtr ambientMap;
        gl::Texture2DPtr specularMap;
        gl::Texture2DPtr alphaMap;
        gl::Texture2DPtr bumpMap;
        glm::vec4 color;
        glm::vec3 emission;
        float shininess;
        float displaceScale;
        float horizontalScale;
    };
    typedef std::vector<Material>   MaterialVec;
}

#endif // !VD_GAME_ENGINE_MATERIAL_HPP

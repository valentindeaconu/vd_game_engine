//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_MATERIAL_HPP
#define VD_GAME_ENGINE_MATERIAL_HPP

#include <glm/glm.hpp>
#include <string>
#include <memory>

#include <engine/api/gl/Texture.hpp>

namespace vd::model {
    class Material {
    public:
        Material();
        explicit Material(std::string name);
        ~Material();

        float& Shininess();
        float& DisplaceScale();
        float& HorizontalScale();

        glm::vec3& Emission();
        glm::vec4& Color();

        std::string& Name();

        gl::Texture2DPtr& DiffuseMap();
        [[nodiscard]] const gl::Texture2DPtr& DiffuseMap() const;

        gl::Texture2DPtr& NormalMap();
        [[nodiscard]] const gl::Texture2DPtr& NormalMap() const;

        gl::Texture2DPtr& DisplaceMap();
        [[nodiscard]] const gl::Texture2DPtr& DisplaceMap() const;

        gl::Texture2DPtr& AmbientMap();
        [[nodiscard]] const gl::Texture2DPtr& AmbientMap() const;

        gl::Texture2DPtr& SpecularMap();
        [[nodiscard]] const gl::Texture2DPtr& SpecularMap() const;

        gl::Texture2DPtr& AlphaMap();
        [[nodiscard]] const gl::Texture2DPtr& AlphaMap() const;

        gl::Texture2DPtr& BumpMap();
        [[nodiscard]] const gl::Texture2DPtr& BumpMap() const;

    private:
        float m_Shininess;
        float m_DisplaceScale;
        float m_HorizontalScale;

        glm::vec3 m_Emission;
        glm::vec4 m_Color;

        std::string m_Name;

        gl::Texture2DPtr m_pDiffuseMap;
        gl::Texture2DPtr m_pNormalMap;
        gl::Texture2DPtr m_pDisplaceMap;
        gl::Texture2DPtr m_pAmbientMap;
        gl::Texture2DPtr m_pSpecularMap;
        gl::Texture2DPtr m_pAlphaMap;
        gl::Texture2DPtr m_pBumpMap;
    };
    typedef std::shared_ptr<Material>   MaterialPtr;
    typedef std::vector<MaterialPtr>    MaterialPtrVec;
}

#endif // !VD_GAME_ENGINE_MATERIAL_HPP

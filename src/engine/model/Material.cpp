//
// Created by Vali on 12/1/2020.
//

#include "Material.hpp"

namespace vd::model {

    Material::Material()
        : m_Name("Unnamed")
        , m_Shininess(0.0f)
        , m_DisplaceScale(0.0f)
        , m_HorizontalScale(0.0f)
        , m_Emission(0.0f)
        , m_Color(0.0f)
    {
    }

    Material::Material(std::string name)
        : m_Name(std::move(name))
        , m_Shininess(0.0f)
        , m_DisplaceScale(0.0f)
        , m_HorizontalScale(0.0f)
        , m_Emission(0.0f)
        , m_Color(0.0f)
    {
    }

    Material::~Material() = default;

    float& Material::Shininess() {
        return m_Shininess;
    }

    float& Material::DisplaceScale() {
        return m_DisplaceScale;
    }

    float& Material::HorizontalScale() {
        return m_HorizontalScale;
    }

    glm::vec3& Material::Emission() {
        return m_Emission;
    }

    glm::vec4& Material::Color() {
        return m_Color;
    }

    std::string& Material::Name() {
        return m_Name;
    }

    gl::Texture2DPtr& Material::DiffuseMap() {
        return m_pDiffuseMap;
    }

    gl::Texture2DPtr& Material::NormalMap() {
        return m_pNormalMap;
    }

    gl::Texture2DPtr& Material::DisplaceMap() {
        return m_pDisplaceMap;
    }

    gl::Texture2DPtr& Material::AmbientMap() {
        return m_pAmbientMap;
    }

    gl::Texture2DPtr& Material::SpecularMap() {
        return m_pSpecularMap;
    }

    gl::Texture2DPtr& Material::AlphaMap() {
        return m_pAlphaMap;
    }

    gl::Texture2DPtr& Material::BumpMap() {
        return m_pBumpMap;
    }

    const gl::Texture2DPtr& Material::DiffuseMap() const {
        return m_pDiffuseMap;
    }

    const gl::Texture2DPtr& Material::NormalMap() const {
        return m_pNormalMap;
    }

    const gl::Texture2DPtr& Material::DisplaceMap() const {
        return m_pDisplaceMap;
    }

    const gl::Texture2DPtr& Material::AmbientMap() const {
        return m_pAmbientMap;
    }

    const gl::Texture2DPtr& Material::SpecularMap() const {
        return m_pSpecularMap;
    }

    const gl::Texture2DPtr& Material::AlphaMap() const {
        return m_pAlphaMap;
    }

    const gl::Texture2DPtr& Material::BumpMap() const {
        return m_pBumpMap;
    }

}

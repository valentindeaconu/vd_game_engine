//
// Created by Vali on 3/26/2021.
//

#ifndef VDGE_BIOME_HPP
#define VDGE_BIOME_HPP

#include <engine/model/Material.hpp>

#include <memory>
#include <string>

#include <modules/props/Prop.hpp>

namespace mod::biomes {
    class Biome {
    public:
        Biome();
        explicit Biome(std::string name);

        std::string& Name();

        float& MinimumHeight();
        float& MaximumHeight();
        float& MinimumSlope();
        float& MaximumSlope();

        vd::model::Material& Material();

        std::vector<vd::object::Entity3DPtr>& Entities();
    private:
        float m_MinimumHeight;
        float m_MaximumHeight;
        float m_MinimumSlope;
        float m_MaximumSlope;

        std::string m_Name;

        vd::model::Material m_Material;

        std::vector<vd::object::Entity3DPtr> m_Entities;
    };
    typedef std::shared_ptr<Biome>  BiomePtr;
    typedef std::vector<BiomePtr>   BiomePtrVec;
}

#endif //VDGE_BIOME_HPP

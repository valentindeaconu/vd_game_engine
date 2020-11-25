//
// Created by Vali on 9/22/2020.
//

#ifndef VD_GAME_ENGINE_BIOME_HPP
#define VD_GAME_ENGINE_BIOME_HPP

#include <engine/model/Material.hpp>

#include <memory>
#include <string>

#include <modules/props/Prop.hpp>

namespace mod::terrain {
    class Biome {
    public:
        Biome();
        explicit Biome(std::string name);
        ~Biome();

        std::string& Name();

        float& MinimumHeight();
        float& MaximumHeight();

        vd::model::Material& Material();

        std::vector<props::PropPtr>& Props();
    private:
        float m_MinimumHeight;
        float m_MaximumHeight;

        std::string m_Name;

        vd::model::Material m_Material;

        std::vector<props::PropPtr> m_Props;
    };
    typedef std::shared_ptr<Biome>  BiomePtr;
    typedef std::vector<BiomePtr>   BiomePtrVec;
}

#endif //VD_GAME_ENGINE_BIOME_HPP

//
// Created by Vali on 9/22/2020.
//

#ifndef VD_GAME_ENGINE_BIOME_HPP
#define VD_GAME_ENGINE_BIOME_HPP

#include <engine/model/Material.hpp>

#include <memory>
#include <string>

#include <modules/sobj/StaticObject.hpp>

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

        std::vector<sobj::StaticObjectPtr>& Objects();
    private:
        float m_MinimumHeight;
        float m_MaximumHeight;

        std::string m_Name;

        vd::model::Material m_Material;

        // TODO: Not like this
        std::vector<sobj::StaticObjectPtr> m_Objects;
    };
    typedef std::shared_ptr<Biome>  BiomePtr;
    typedef std::vector<BiomePtr>   BiomePtrVec;
}

#endif //VD_GAME_ENGINE_BIOME_HPP

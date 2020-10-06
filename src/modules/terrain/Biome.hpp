//
// Created by Vali on 9/22/2020.
//

#ifndef VD_GAME_ENGINE_BIOME_HPP
#define VD_GAME_ENGINE_BIOME_HPP

#include <engine/model/Material.hpp>

#include <memory>
#include <string>

namespace mod::terrain {
    class Biome {
    public:
        Biome();
        explicit Biome(const std::string& name);
        ~Biome();

        [[nodiscard]] const std::string& getName() const;
        void setName(const std::string& name);

        [[nodiscard]] float getMinHeight() const;
        void setMinHeight(float minHeight);

        [[nodiscard]] float getMaxHeight() const;
        void setMaxHeight(float maxHeight);

        [[nodiscard]] vd::model::Material& getMaterial();
        [[nodiscard]] const vd::model::Material& getMaterial() const;
        void setMaterial(const vd::model::Material& material);
    private:
        float minHeight;
        float maxHeight;

        std::string name;

        vd::model::Material material;
    };
    typedef std::shared_ptr<Biome>  BiomePtr;
    typedef std::vector<BiomePtr>   BiomePtrVec;
}

#endif //VD_GAME_ENGINE_BIOME_HPP

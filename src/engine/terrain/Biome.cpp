//
// Created by Vali on 9/22/2020.
//

#include "Biome.hpp"

namespace vd::terrain {

    Biome::Biome()
        : minHeight(0.0f)
        , maxHeight(0.0f)
        , name()
        , material()
    {
    }

    Biome::Biome(const std::string& name)
        : minHeight(0.0f)
        , maxHeight(0.0f)
        , name(name)
        , material()
    {
    }

    Biome::~Biome() = default;

    const std::string &Biome::getName() const {
        return name;
    }

    void Biome::setName(const std::string& name) {
        this->name = name;
    }

    float Biome::getMinHeight() const {
        return minHeight;
    }

    void Biome::setMinHeight(float minHeight) {
        this->minHeight = minHeight;
    }

    float Biome::getMaxHeight() const {
        return maxHeight;
    }

    void Biome::setMaxHeight(float maxHeight) {
        this->maxHeight = maxHeight;
    }

    model::Material& Biome::getMaterial() {
        return material;
    }

    const model::Material& Biome::getMaterial() const {
        return material;
    }

    void Biome::setMaterial(const model::Material& material) {
        this->material = material;
    }
}
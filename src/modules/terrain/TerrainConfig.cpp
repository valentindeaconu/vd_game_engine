//
// Created by Vali on 9/21/2020.
//

#include "TerrainConfig.hpp"

namespace mod::terrain {

    TerrainConfig::TerrainConfig(const std::string &filePath)
        : ConfigurationFile(filePath)
        , scaleY(0.0f)
        , scaleXZ(0.0f)
        , lodRange()
        , lodMorphingArea()
    {
    }

    TerrainConfig::~TerrainConfig() = default;

    void TerrainConfig::onTokenReceived(const std::string &command, const std::vector<std::string> &tokens) {
        if (command == "rootNodes") {
            rootNodes = std::stoi(tokens[0]);
        }
        else if (command == "scaleY") {
            scaleY = std::stof(tokens[0]);
        }
        else if (command == "scaleXZ") {
            scaleXZ = std::stof(tokens[0]);
        }
        else if (command == "highDetailRange") {
            highDetailRange = std::stoi(tokens[0]);
        }
        else if (command == "tessellationFactor") {
            tessellationFactor = std::stoi(tokens[0]);
        }
        else if (command == "tessellationSlope") {
            tessellationSlope = std::stof(tokens[0]);
        }
        else if (command == "tessellationShift") {
            tessellationShift = std::stof(tokens[0]);
        }
        else if (command == "tessellationOuterLevel") {
            tessellationOuterLevel = std::stof(tokens[0]);
        }
        else if (command == "tessellationInnerLevel") {
            tessellationInnerLevel = std::stof(tokens[0]);
        }
        else if (command == "levelOfDetailEnabled") {
            levelOfDetailEnabled = std::stoi(tokens[0]);
        }
        else if (command == "heightmap") {
            vd::img::IMGLoader imgLoader;
            heightImg = imgLoader.loadFloatImage(tokens[0]);
            heightMap = std::make_shared<vd::model::Texture2D>(heightImg);
        }
        else if (command == "normalStrength") {
            normalStrength = std::stof(tokens[0]);
        }
        else if (command.starts_with("lod") && command.ends_with("_range")) {
            int index = std::stoi(command.substr(3)) - 1;

            if (index >= kDetailLevels) {
                return;
            }

            int lodRangeValue = std::stoi(tokens[0]);

            if (lodRangeValue == 0) {
                lodRange[index] = lodMorphingArea[index] = 0;
            } else {
                setLodRange(index, lodRangeValue);
            }
        }
        else if (command == "biome") {
            biomes.push_back(std::make_shared<Biome>());
        }
        else if (command == "name") {
            biomes.back()->setName(tokens[0]);
        }
        else if (command == "min_height") {
            float height = std::stof(tokens[0]) * scaleY;
            biomes.back()->setMinHeight(height);
        }
        else if (command == "max_height") {
            float height = std::stof(tokens[0]) * scaleY;
            biomes.back()->setMaxHeight(height);
        }
        else if (command == "material_DIF") {
            vd::model::Material& material = biomes.back()->getMaterial();
            material.diffuseMap = vd::model::TextureService::get(tokens[0]);
            material.diffuseMap->bind();
            material.diffuseMap->trilinearFilter();
            material.diffuseMap->unbind();
        }
        else if (command == "material_NRM") {
            vd::model::Material& material = biomes.back()->getMaterial();
            material.normalMap = vd::model::TextureService::get(tokens[0]);
            material.normalMap->bind();
            material.normalMap->bilinearFilter();
            material.normalMap->unbind();
        }
        else if (command == "material_DISP") {
            vd::model::Material& material = biomes.back()->getMaterial();
            material.displaceMap = vd::model::TextureService::get(tokens[0]);
            material.displaceMap->bind();
            material.displaceMap->bilinearFilter();
            material.displaceMap->unbind();
        }
        else if (command == "material_heightScaling") {
            vd::model::Material& material = biomes.back()->getMaterial();
            material.displaceScale = std::stof(tokens[0]);
        }
        else if (command == "material_horizontalScaling") {
            vd::model::Material& material = biomes.back()->getMaterial();
            material.horizontalScale = std::stof(tokens[0]);
        }
        else if (command == "object") {
            sobj::StaticObjectPtr staticObjectPtr = std::make_shared<sobj::StaticObject>(tokens[0], tokens[1]);
            float scaleFactor = std::stof(tokens[2]);

            staticObjectPtr->init();
            staticObjectPtr->getWorldTransform().setScaling(scaleFactor, scaleFactor, scaleFactor);

            biomes.back()->addObject(staticObjectPtr);
        }

    }

    void TerrainConfig::onParseFinish() {
        normalmap::NormalMapRendererPtr normalMapRendererPtr =
                std::make_shared<normalmap::NormalMapRenderer>(int(heightMap->getWidth()));
        normalMapRendererPtr->render(heightMap, normalStrength);
        normalMap = normalMapRendererPtr->getNormalMap();

        splatmap::SplatMapRendererPtr splatMapRendererPtr =
                std::make_shared<splatmap::SplatMapRenderer>(int(heightMap->getWidth()));
        splatMapRendererPtr->render(heightMap, scaleY, biomes);
        splatMap = splatMapRendererPtr->getSplatMap();
        splatImg = splatMapRendererPtr->getSplatData();

        vd::math::Transform transform;
        transform.setScaling(scaleXZ, scaleY, scaleXZ);
        transform.setTranslation(-scaleXZ/2.0f, 0.0f, -scaleXZ/2.0f);

        this->worldTransform = transform.get();
        this->inverseWorldTransform = transform.inverse();
    }

    int TerrainConfig::updateMorphingArea(int lod) const {
        return (int) ((scaleXZ / rootNodes) / float(1 << lod));
    }

    void TerrainConfig::setLodRange(int index, int lodRangeValue) {
        lodRange[index] = lodRangeValue;
        lodMorphingArea[index] = lodRangeValue - updateMorphingArea(index + 1);
    }

    int TerrainConfig::getRootNodes() const {
        return rootNodes;
    }

    float TerrainConfig::getScaleY() const {
        return scaleY;
    }

    float TerrainConfig::getScaleXZ() const {
        return scaleXZ;
    }

    const TerrainConfig::LodVec& TerrainConfig::getLodRange() const {
        return lodRange;
    }

    const TerrainConfig::LodVec& TerrainConfig::getLodMorphingArea() const {
        return lodMorphingArea;
    }

    int TerrainConfig::getTessellationFactor() const {
        return tessellationFactor;
    }

    float TerrainConfig::getTessellationSlope() const {
        return tessellationSlope;
    }

    float TerrainConfig::getTessellationShift() const {
        return tessellationShift;
    }

    float TerrainConfig::getTessellationOuterLevel() const {
        return tessellationOuterLevel;
    }

    float TerrainConfig::getTessellationInnerLevel() const {
        return tessellationInnerLevel;
    }

    int TerrainConfig::getHighDetailRange() const {
        return highDetailRange;
    }

    int TerrainConfig::getMaxDetailLevel() const {
        return kDetailLevels - 1;
    }

    bool TerrainConfig::isLevelOfDetailEnabled() const {
        return levelOfDetailEnabled;
    }

    const vd::model::Texture2DPtr& TerrainConfig::getHeightMap() const {
        return heightMap;
    }

    const vd::model::Texture2DPtr& TerrainConfig::getNormalMap() const {
        return normalMap;
    }

    const vd::model::Texture2DPtr& TerrainConfig::getSplatMap() const {
        return splatMap;
    }

    const vd::img::ImageFPtr& TerrainConfig::getHeightImg() const {
        return heightImg;
    }

    const glm::mat4& TerrainConfig::getTransform() const {
        return worldTransform;
    }

    const glm::mat4& TerrainConfig::getInverseTransform() const {
        return inverseWorldTransform;
    }

    const BiomePtrVec& TerrainConfig::getBiomes() const {
        return biomes;
    }

    BiomePtrVec TerrainConfig::getBiomesAt(float x, float z) const {
        float upperBound = scaleXZ / 2.0f;
        float lowerBound = -upperBound;

        BiomePtrVec output;

        if (x < lowerBound || z < lowerBound || x >= upperBound || z >= upperBound)
            return output;

        // reverse transform
        float rx = (x + (scaleXZ / 2.0f)) / scaleXZ;
        float rz = (z + (scaleXZ / 2.0f)) / scaleXZ;

        const auto mask = vd::img::ImageHelper::texture(*splatImg, glm::vec2(rz, rx)).r;

        for (size_t k = 0; k < biomes.size(); ++k) {
            if ((mask & (1 << k)) != 0) {
                output.emplace_back(biomes[k]);
            }
        }

        return output;
    }

    float TerrainConfig::getHeight(float x, float z) const {
        float upperBound = scaleXZ / 2.0f;
        float lowerBound = -upperBound;

        if (x < lowerBound || z < lowerBound || x >= upperBound || z >= upperBound)
            return 0.0f;

        // reverse transform
        float rx = (x + (scaleXZ / 2.0f)) / scaleXZ;
        float rz = (z + (scaleXZ / 2.0f)) / scaleXZ;

        const auto height = vd::img::ImageHelper::texture(*heightImg, glm::vec2(rz, rx)).r;

        return height * scaleY;
    }
}
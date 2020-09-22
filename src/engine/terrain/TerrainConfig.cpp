//
// Created by Vali on 9/21/2020.
//

#include "TerrainConfig.hpp"

namespace vd::terrain {

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
        else if (command == "tessellationFactor") {
            tessellationFactor = std::stoi(tokens[0]);
        }
        else if (command == "tessellationSlope") {
            tessellationSlope = std::stof(tokens[0]);
        }
        else if (command == "tessellationShift") {
            tessellationShift = std::stof(tokens[0]);
        }
        else if (command == "heightmap") {
            imgloader::IMGLoader imgLoader;
            heightImg = imgLoader.loadFloatImage(tokens[0]);

            heightMap = model::TextureService::get(tokens[0]);
            heightMap->bind();
            heightMap->trilinearFilter();
            heightMap->unbind();
        }
        else if (command == "normalStrength") {
            normalStrength = std::stof(tokens[0]);
        }
        else if (command.starts_with("lod") && command.ends_with("_range")) {
            int index = command[3] - '0' - 1;

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
            biomes.back()->setMinHeight(std::stof(tokens[0]));
        }
        else if (command == "max_height") {
            biomes.back()->setMaxHeight(std::stof(tokens[0]));
        }
        else if (command == "material_DIF") {
            model::Material& material = biomes.back()->getMaterial();
            material.diffuseMap = model::TextureService::get(tokens[0]);
            material.diffuseMap->bind();
            material.diffuseMap->trilinearFilter();
            material.diffuseMap->unbind();
        }
        else if (command == "material_NRM") {
            model::Material& material = biomes.back()->getMaterial();
            material.normalMap = model::TextureService::get(tokens[0]);
            material.normalMap->bind();
            material.normalMap->trilinearFilter();
            material.normalMap->unbind();
        }
        else if (command == "material_DISP") {
            model::Material& material = biomes.back()->getMaterial();
            material.displaceMap = model::TextureService::get(tokens[0]);
            material.displaceMap->bind();
            material.displaceMap->trilinearFilter();
            material.displaceMap->unbind();
        }
        else if (command == "material_heightScaling") {
            model::Material& material = biomes.back()->getMaterial();
            material.displaceScale = std::stof(tokens[0]);
        }
        else if (command == "material_horizontalScaling") {
            model::Material& material = biomes.back()->getMaterial();
            material.horizontalScale = std::stof(tokens[0]);
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

    int TerrainConfig::getTbnRange() const {
        return tbnRange;
    }

    const model::Texture2DPtr& TerrainConfig::getHeightMap() const {
        return heightMap;
    }

    const model::Texture2DPtr& TerrainConfig::getNormalMap() const {
        return normalMap;
    }

    const model::Texture2DPtr& TerrainConfig::getSplatMap() const {
        return splatMap;
    }

    const BiomePtrVec& TerrainConfig::getBiomes() const {
        return biomes;
    }

    float TerrainConfig::getHeight(float x, float z) const {
        if (x < 0.0f || z < 0.0f || x >= scaleXZ || z >= scaleXZ)
            return 0.0f;

        // reverse scaling
        float rx = x / scaleXZ;
        float rz = z / scaleXZ;

        // upscale to heightmap
        float hx = rx * heightImg->width;
        float hz = rz * heightImg->width;

        // compute integer coordinates
        int x0 = int(glm::floor(hx));
        int x1 = x0 + 1;
        int z0 = int(glm::floor(hz));
        int z1 = z0 + 1;

        if (hx == float(x0) && hz == float(z0))
            return heightImg->at(z0, x0).r * scaleY;

        float h0 = heightImg->at(z0, x0).r;
        float h1 = heightImg->at(z0, x1).r;
        float h2 = heightImg->at(z1, x0).r;
        float h3 = heightImg->at(z1, x1).r;

        float frac_x = hx - glm::floor(hx);
        float frac_z = hz - glm::floor(hz);

        float h01 = glm::mix(h0, h1, frac_x);
        float h23 = glm::mix(h2, h3, frac_x);

        float h = glm::mix(h01, h23, frac_z);
        h *= scaleY;

        return h;
    }
}
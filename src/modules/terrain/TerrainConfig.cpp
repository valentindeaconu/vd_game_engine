#include "TerrainConfig.hpp"

namespace mod::terrain
{
    TerrainConfig::TerrainConfig(const std::string& filePath)
        : ConfigurationFile(filePath)
    {
        imgLoaderPtr = std::make_shared<vd::imgloader::IMGLoader>();
    }

    TerrainConfig::~TerrainConfig() = default;

    void TerrainConfig::initializeObjects(const vd::EnginePtr& enginePtr)
    {
        for (auto it = biomeAtlas.begin(); it != biomeAtlas.end(); ++it)
        {
            for (auto jt = it->objects.begin(); jt != it->objects.end(); ++jt)
            {
                (*jt)->setParentEngine(enginePtr);
            }
        }
    }

    size_t TerrainConfig::getSize() const
    {
        return size;
    }

    const BiomeAtlas& TerrainConfig::getBiomeAtlas() const
    {
        return biomeAtlas;
    }

    vd::model::UTexture2DPtr TerrainConfig::getSplatmap() const
    {
        return blendmap.texture;
    }

    float TerrainConfig::getMaxHeight() const
    {
        return maxHeight;
    }

    float TerrainConfig::getNormalStrength() const {
        return normalStrength;
    }

    float TerrainConfig::getHeight(float x, float z) const
    {
        // TODO: Take care of this
        std::swap(x, z);

        int _x = glm::floor(x);
        int _z = glm::floor(z);

        if (_x < 0 || _z < 0 || _x > heightmap->height || _z > heightmap->width)
        {
            return 0.0f;
        }

        if (glm::abs(_x - x) < 0.0001f && glm::abs(_z - z) < 0.0001f)
        {
            vd::imgloader::PixelF pixel = heightmap->at(_x, _z);

            float h = pixel.r; //(float)pixel.r / 255.0f;
            h *= 2.0f;
            h -= 1.0f;
            h *= maxHeight;

            return h;
        }
        else
        {
            float h0 = getHeight(_x, _z);
            float h1 = getHeight(_x + 1, _z);

            float h2 = getHeight(_x, _z + 1);
            float h3 = getHeight(_x + 1, _z + 1);

            float frac_x = x - (float)_x;
            float h01 = glm::mix(h0, h1, frac_x);
            float h23 = glm::mix(h2, h3, frac_x);

            float frac_z = z - (float)_z;
            float h = glm::mix(h01, h23, frac_z);
            return h;
        }

        return 0.0f;
    }

    std::string TerrainConfig::getBiome(float x, float z) const
    {
        std::swap(x, z);

        int _x = glm::floor(x);
        int _z = glm::floor(z);

        if (_x < 0 || _z < 0 || _x > heightmap->height || _z > heightmap->width)
            return "Unknown";


        int index = _x * heightmap->width + _z;
        uint16_t mask = blendmap.data[index];

        for (size_t k = 0; k < biomeAtlas.size(); ++k)
        {
            if ((mask & (1 << k)) != 0)
            {
                return biomeAtlas[k].name;
            }
        }

        return "Unknown";
    }

    size_t TerrainConfig::getBiomeIndex(float x, float z) const
    {
        const std::string& biomeName = getBiome(x, z);

        auto it = biomeIndices.find(biomeName);
        if (it != biomeIndices.end()) {
            return it->second;
        }

        return 0;
    }

    void TerrainConfig::onTokenReceived(const std::string& command, const std::vector<std::string>& tokens)
    {
        if (command == "heightmap")
        {
            heightmap = imgLoaderPtr->loadFloatImage(tokens[0]);
            size = heightmap->height;
        }
        else if (command == "maxHeight")
        {
            maxHeight = std::stof(tokens[0]);
        }
        else if (command == "normalStrength")
        {
            normalStrength = std::stof(tokens[0]);
        }
        else if (command == "biome")
        {
            biomeAtlas.push_back(Biome());
        }
        else if (command == "name")
        {
            biomeAtlas.back().name = tokens[0];
            biomeIndices[tokens[0]] = biomeAtlas.size() - 1;
        }
        else if (command == "map_DIF")
        {
            biomeAtlas.back().material.diffusemap = vd::model::TextureService::get(tokens[0]);
            biomeAtlas.back().material.diffusemap->bind();
            biomeAtlas.back().material.diffusemap->trilinearFilter();
        }
        else if (command == "height_threshold_min")
        {
            biomeAtlas.back().height.min = std::stof(tokens[0]);
        }
        else if (command == "height_threshold_max")
        {
            biomeAtlas.back().height.max = std::stof(tokens[0]);
        }
        else if (command == "object")
        {
            sobj::StaticObjectPtr staticObjectPtr = std::make_shared<sobj::StaticObject>(tokens[0], tokens[1]);
            float scaleFactor = std::stof(tokens[2]);

            staticObjectPtr->init();
            staticObjectPtr->getWorldTransform().setScaling(scaleFactor, scaleFactor, scaleFactor);

            biomeAtlas.back().objects.push_back(staticObjectPtr);
        }
    }

    void TerrainConfig::onParseFinish()
    {
        generateBlendmap();

        // release imgLoader since it is not necessary anymore
        imgLoaderPtr = nullptr;
    }

    void TerrainConfig::generateBlendmap()
    {
        if (heightmap == nullptr || heightmap->width == 0 || heightmap->height == 0 || biomeAtlas.empty())
        {
            blendmap.texture = nullptr;

            vd::Logger::terminate("could not create blendmap", 1);
        }

        blendmap.data.reserve(heightmap->width * heightmap->height);

        for (size_t i = 0; i < heightmap->height; ++i)
        {
            for (size_t j = 0; j < heightmap->width; ++j)
            {
                float h = getHeight(i, j);

                uint16_t mask = 0;
                for (size_t k = 0; k < biomeAtlas.size(); ++k)
                {
                    if (biomeAtlas[k].height.min <= h && h <= biomeAtlas[k].height.max)
                    {
                        // set bit on position K to mark that we use the k-th texture
                        mask |= (1 << k);
                    }
                }

                blendmap.data.push_back(mask);
            }
        }

        blendmap.texture = vd::model::TextureService::get(heightmap->width, heightmap->height, blendmap.data);
    }
}
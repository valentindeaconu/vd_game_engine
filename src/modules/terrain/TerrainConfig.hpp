#ifndef __TERRAIN_CONFIG_HPP_
#define __TERRAIN_CONFIG_HPP_

#include <engine/config/ConfigurationFile.hpp>

#include <engine/foundation/imgloader/IMGLoader.hpp>
#include <engine/model/Texture.hpp>
#include <engine/model/Material.hpp>

#include <modules/sobj/StaticObject.hpp>

namespace mod::terrain
{
    struct Biome
    {
        std::string name;
        struct { float min, max; } height;
        vd::model::Material material;
        std::vector<sobj::StaticObjectPtr> objects;
    };
    typedef std::vector<Biome>	BiomeAtlas;

    class TerrainConfig : public vd::config::ConfigurationFile
    {
    public:
        TerrainConfig(const std::string& filePath);
        ~TerrainConfig();

        void initializeObjects(const vd::EnginePtr& enginePtr);

        size_t getSize() const;

        const BiomeAtlas& getBiomeAtlas() const;

        vd::model::UTexture2DPtr getSplatmap() const;

        float getMaxHeight() const;

        float getNormalStrength() const;

        float getHeight(float x, float z) const;
        std::string getBiome(float x, float z) const;
        size_t getBiomeIndex(float x, float z) const;
    private:
        void onTokenReceived(const std::string& key, const std::vector<std::string>& tokens) override;
        void onParseFinish() override;

        void generateBlendmap();

        vd::imgloader::IMGLoaderPtr imgLoaderPtr;

        size_t size;

        float maxHeight;

        float normalStrength;

        BiomeAtlas biomeAtlas;

        std::unordered_map<std::string, size_t> biomeIndices;

        struct
        {
            vd::model::UTexture2DPtr texture;
            std::vector<uint16_t> data;
        } blendmap;

        vd::imgloader::ImageFPtr heightmap;
    };
    typedef std::shared_ptr<TerrainConfig>	TerrainConfigPtr;
}

#endif // !__TERRAIN_CONFIG_HPP_

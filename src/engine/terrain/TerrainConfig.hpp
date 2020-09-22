//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAINCONFIG_HPP
#define VD_GAME_ENGINE_TERRAINCONFIG_HPP

#include <engine/config/ConfigurationFile.hpp>
#include <engine/model/Texture.hpp>
#include <engine/foundation/imgloader/IMGLoader.hpp>

#include <array>
#include <memory>

#include "Biome.hpp"

#include "normalmap/NormalMapRenderer.hpp"
#include "splatmap/SplatMapRenderer.hpp"

namespace vd::terrain {
    class TerrainConfig : public vd::config::ConfigurationFile {
    public:
        typedef std::array<int, 8> LodVec;

        explicit TerrainConfig(const std::string& filePath);
        ~TerrainConfig();

        [[nodiscard]] int getRootNodes() const;

        [[nodiscard]] float getScaleY() const;
        [[nodiscard]] float getScaleXZ() const;

        [[nodiscard]] const LodVec& getLodRange() const;
        [[nodiscard]] const LodVec& getLodMorphingArea() const;

        [[nodiscard]] int getTessellationFactor() const;
        [[nodiscard]] float getTessellationSlope() const;
        [[nodiscard]] float getTessellationShift() const;

        [[nodiscard]] int getTbnRange() const;

        [[nodiscard]] const model::Texture2DPtr& getHeightMap() const;
        [[nodiscard]] const model::Texture2DPtr& getNormalMap() const;
        [[nodiscard]] const model::Texture2DPtr& getSplatMap() const;

        [[nodiscard]] const BiomePtrVec& getBiomes() const;

        [[nodiscard]] float getHeight(float x, float z) const;
    private:
        void onTokenReceived(const std::string& command, const std::vector<std::string>& tokens) override;
        void onParseFinish() override;

        [[nodiscard]] int updateMorphingArea(int lod) const;
        void setLodRange(int index, int lodRangeValue);

        int rootNodes;

        float scaleY;
        float scaleXZ;

        int tessellationFactor;
        float tessellationSlope;
        float tessellationShift;

        int tbnRange;

        float normalStrength;

        BiomePtrVec biomes;

        imgloader::ImageFPtr heightImg;

        model::Texture2DPtr heightMap;
        model::Texture2DPtr normalMap;
        model::Texture2DPtr splatMap;

        std::array<int, 8> lodRange;
        std::array<int, 8> lodMorphingArea;

    };
    typedef std::shared_ptr<TerrainConfig>  TerrainConfigPtr;
}


#endif //VD_GAME_ENGINE_TERRAINCONFIG_HPP

//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAINCONFIG_HPP
#define VD_GAME_ENGINE_TERRAINCONFIG_HPP

#include <engine/config/ConfigurationFile.hpp>
#include <engine/model/Texture.hpp>
#include <engine/foundation/img/Image.hpp>
#include <engine/foundation/img/imgloader/IMGLoader.hpp>
#include <engine/foundation/img/imghelper/ImageHelper.hpp>
#include <engine/foundation/math/Transform.hpp>

#include <array>
#include <memory>

#include "Biome.hpp"

#include "normalmap/NormalMapRenderer.hpp"
#include "splatmap/SplatMapRenderer.hpp"

namespace mod::terrain {
    class TerrainConfig : public vd::config::ConfigurationFile {
    public:
        static const int kDetailLevels = 8;
        typedef std::array<int, kDetailLevels> LodVec;

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

        [[nodiscard]] float getTessellationOuterLevel() const;
        [[nodiscard]] float getTessellationInnerLevel() const;

        [[nodiscard]] bool isLevelOfDetailEnabled() const;

        [[nodiscard]] int getHighDetailRange() const;

        [[nodiscard]] int getMaxDetailLevel() const;

        [[nodiscard]] const vd::model::Texture2DPtr& getHeightMap() const;
        [[nodiscard]] const vd::model::Texture2DPtr& getNormalMap() const;
        [[nodiscard]] const vd::model::Texture2DPtr& getSplatMap() const;

        [[nodiscard]] const vd::img::ImageFPtr& getHeightImg() const;

        [[nodiscard]] const glm::mat4& getTransform() const;
        [[nodiscard]] const glm::mat4& getInverseTransform() const;

        [[nodiscard]] const BiomePtrVec& getBiomes() const;

        [[nodiscard]] BiomePtrVec getBiomesAt(float x, float z) const;

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

        float tessellationOuterLevel;
        float tessellationInnerLevel;

        bool levelOfDetailEnabled;

        int highDetailRange;

        float normalStrength;

        BiomePtrVec biomes;

        vd::img::ImageFPtr heightImg;
        vd::img::RawFloatImagePtr heightData;

        vd::model::Texture2DPtr heightMap;
        vd::model::Texture2DPtr normalMap;

        vd::model::Texture2DPtr splatMap;
        vd::img::ImageIPtr splatImg;

        std::array<int, kDetailLevels> lodRange;
        std::array<int, kDetailLevels> lodMorphingArea;

        glm::mat4 worldTransform;
        glm::mat4 inverseWorldTransform;
    };
    typedef std::shared_ptr<TerrainConfig>  TerrainConfigPtr;
}


#endif //VD_GAME_ENGINE_TERRAINCONFIG_HPP

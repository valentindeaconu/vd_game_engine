//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_SPLATMAPRENDERER_HPP
#define VD_GAME_ENGINE_SPLATMAPRENDERER_HPP

#include <memory>

#include "../Biome.hpp"

#include "SplatMapShader.hpp"

namespace mod::terrain::splatmap {
    class SplatMapRenderer {
    public:
        explicit SplatMapRenderer(int size);
        ~SplatMapRenderer();

        void render(const vd::model::Texture2DPtr& normalMap, float scaleY, const BiomePtrVec& biomes);

        [[nodiscard]] const vd::model::Texture2DPtr& getSplatMap() const;

        [[nodiscard]] const vd::img::ImageIPtr& getSplatData() const;
    private:
        const int kBiomeCount;

        SplatMapShaderPtr shaderPtr;

        vd::model::Texture2DPtr splatMap;
        vd::img::ImageIPtr      splatImg;

        int size;
    };
    typedef std::shared_ptr<SplatMapRenderer>   SplatMapRendererPtr;
}


#endif //VD_GAME_ENGINE_SPLATMAPRENDERER_HPP

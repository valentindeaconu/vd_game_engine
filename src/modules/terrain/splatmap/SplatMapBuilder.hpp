//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_SPLATMAPBUILDER_HPP
#define VD_GAME_ENGINE_SPLATMAPBUILDER_HPP

#include <engine/api/gl/Texture.hpp>
#include <engine/api/gl/Shader.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include <engine/service/TextureService.hpp>

#include <memory>

#include "../Biome.hpp"

namespace mod::terrain::splatmap {
    class SplatMapBuilder {
    public:
        typedef vd::model::ImagePtr<uint32_t, vd::model::ImageFormat::eR>   data_t;

        SplatMapBuilder();
        ~SplatMapBuilder();

        void Create(const vd::gl::Texture2DPtr& heightMap,
                    int size,
                    float scaleY,
                    const BiomePtrVec& biomes,
                    vd::gl::Texture2DPtr& outSplatMap,
                    data_t& outData);
    private:
        vd::gl::ShaderPtr m_pShader;
    };
    typedef std::shared_ptr<SplatMapBuilder>   SplatMapBuilderPtr;
}


#endif //VD_GAME_ENGINE_SPLATMAPBUILDER_HPP

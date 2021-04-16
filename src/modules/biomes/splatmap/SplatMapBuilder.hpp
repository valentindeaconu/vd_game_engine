//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_SPLATMAPBUILDER_HPP
#define VD_GAME_ENGINE_SPLATMAPBUILDER_HPP

#include <engine/gapi/gl/Texture.hpp>
#include <engine/gapi/gl/wrappers/Shader.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include <engine/service/TextureService.hpp>

#include <memory>

#include "../Biome.hpp"

namespace mod::biomes::splatmap {
    class SplatMapBuilder {
    public:
        typedef vd::model::ImagePtr<float, vd::model::ImageFormat::eRGBA>   data_t;

        SplatMapBuilder();

        void Create(const vd::gl::Texture2DPtr& heightMap,
                    const vd::gl::Texture2DPtr& normalMap,
                    int size,
                    const BiomePtrVec& biomes,
                    vd::gl::Texture2DPtr& outSplatMap,
                    SplatMapBuilder::data_t& outData);
        void CleanUp();
    private:
        vd::gl::wrappers::ShaderPtr m_pShader;
    };
    typedef std::shared_ptr<SplatMapBuilder>   SplatMapBuilderPtr;
}


#endif //VD_GAME_ENGINE_SPLATMAPBUILDER_HPP

//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_NORMALMAPBUILDER_HPP
#define VD_GAME_ENGINE_NORMALMAPBUILDER_HPP

#include <engine/api/gl/Texture.hpp>
#include <engine/api/gl/Shader.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include <engine/service/TextureService.hpp>

#include <memory>

namespace mod::terrain::normalmap {
    class NormalMapBuilder {
    public:
        NormalMapBuilder();
        ~NormalMapBuilder();

        void Create(const vd::gl::Texture2DPtr& heightMap, int size, float strength, vd::gl::Texture2DPtr& outNormalMap);
    private:
        vd::gl::ShaderPtr m_pShader;
    };
    typedef std::shared_ptr<NormalMapBuilder> NormalMapBuilderPtr;
}


#endif //VD_GAME_ENGINE_NORMALMAPBUILDER_HPP

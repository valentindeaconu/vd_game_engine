//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_NORMALMAPRENDERER_HPP
#define VD_GAME_ENGINE_NORMALMAPRENDERER_HPP

#include <memory>

#include "NormalMapShader.hpp"

namespace vd::terrain::normalmap {
    class NormalMapRenderer {
    public:
        explicit NormalMapRenderer(int size);
        ~NormalMapRenderer();

        void render(const model::Texture2DPtr& heightMap, float strength);

        [[nodiscard]] const model::Texture2DPtr& getNormalMap() const;
    private:
        NormalMapShaderPtr shaderPtr;

        model::Texture2DPtr normalMap;

        int size;
    };
    typedef std::shared_ptr<NormalMapRenderer> NormalMapRendererPtr;
}


#endif //VD_GAME_ENGINE_NORMALMAPRENDERER_HPP

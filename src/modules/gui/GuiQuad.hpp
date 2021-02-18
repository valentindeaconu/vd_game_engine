//
// Created by Vali on 8/29/2020.
//

#ifndef VD_GAME_ENGINE_GUIQUAD_HPP
#define VD_GAME_ENGINE_GUIQUAD_HPP

#include <engine/object/Entity2D.hpp>

#include <memory>

namespace mod::gui {
    class GuiQuad : public vd::object::Entity2D {
    public:
        typedef std::function<vd::gl::Texture2DPtr()> TextureGetter;

        GuiQuad(TextureGetter textureGetter, const glm::vec2& position, const glm::vec2& scale);
        GuiQuad(vd::gl::Texture2DPtr texture2DPtr, const glm::vec2& position, const glm::vec2& scale);
        ~GuiQuad();

        void Setup() override;
        void Update() override;
    private:
        TextureGetter m_TextureGetter;
        vd::gl::Texture2DPtr m_Texture;
    };
    typedef std::shared_ptr<GuiQuad>    GuiQuadPtr;
}



#endif //VD_GAME_ENGINE_GUIQUAD_HPP

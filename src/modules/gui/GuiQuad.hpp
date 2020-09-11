//
// Created by Vali on 8/29/2020.
//

#ifndef VD_GAME_ENGINE_GUIQUAD_HPP
#define VD_GAME_ENGINE_GUIQUAD_HPP

#include <engine/object/Entity.hpp>

#include <memory>

namespace mod::gui {
    class GuiQuad : public vd::object::Entity {
    public:
        GuiQuad(const vd::EnginePtr &enginePtr,
                const vd::model::Texture2DPtr& texture,
                const glm::vec2& position,
                const glm::vec2& scale);
        ~GuiQuad();

        void init() override;
        void update() override;
        void cleanUp() override;
    private:
        vd::model::Texture2DPtr texture;
    };
    typedef std::shared_ptr<GuiQuad>    GuiQuadPtr;
}



#endif //VD_GAME_ENGINE_GUIQUAD_HPP

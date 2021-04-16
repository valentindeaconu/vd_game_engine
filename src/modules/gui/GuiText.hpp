//
// Created by Vali on 2/18/2021.
//

#ifndef VDGE_GUITEXT_HPP
#define VDGE_GUITEXT_HPP

#include <engine/object/Entity2D.hpp>
#include <engine/model/Font.hpp>

#include <engine/gapi/gl/wrappers/Buffer.hpp>

#include <engine/core/Threads.hpp>

namespace mod::gui {
    class GuiText : public vd::object::Entity2D {
    public:
        typedef std::array<vd::model::Vertex, 6>    LetterMesh;
        typedef std::vector<LetterMesh>             LetterMeshVec;

        GuiText(std::string text,
                vd::model::FontPtr font,
                const glm::vec2& position,
                float scale,
                const glm::vec3& color = glm::vec3(1.0f));

        void Setup() override;
        void Init() override;
        virtual void Update();

        std::string& Text();
        vd::model::FontPtr& Font();
        float& Scale();
        glm::vec2& Position();
        LetterMeshVec& LetterMeshes();

        void Color(const glm::vec3& color);

        void Rebuild();
    private:
        vd::model::FontPtr  m_pFont;
        float               m_Scale;
        glm::vec2           m_Position;
        glm::vec3           m_Color;
        std::string         m_Text;
        LetterMeshVec       m_LetterMeshes;
    };
    typedef std::shared_ptr<GuiText>    GuiTextPtr;

    class UpdatableGuiText : public GuiText {
    public:
        typedef std::function<void(UpdatableGuiText&)>  UpdateConsumer;

        UpdatableGuiText(std::string text,
                         vd::model::FontPtr font,
                         UpdateConsumer updateConsumer,
                         const glm::vec2& position,
                         float scale,
                         const glm::vec3& color = glm::vec3(1.0f));
        void Update() override;

        bool Contains(const std::string& param) const;
        std::string& operator[](const std::string& param);
    private:
        UpdateConsumer                                  m_UpdateConsumer;
        std::unordered_map<std::string, std::string>    m_Params;
    };
    typedef std::shared_ptr<UpdatableGuiText>   UpdatableGuiTextPtr;
}


#endif //VDGE_GUITEXT_HPP

//
// Created by Vali on 2/17/2021.
//

#ifndef VDGE_FONT_HPP
#define VDGE_FONT_HPP

#include <memory>
#include <string>
#include <map>

#include <glm/glm.hpp>
#include <engine/defines/Types.hpp>
#include <engine/gapi/gl/Texture.hpp>

namespace vd::model {
    struct Character {
        gl::Texture2DPtr    Texture;    // Handler of the glyph texture
        glm::ivec2          Size;       // Size of glyph
        glm::ivec2          Bearing;    // Offset from baseline to left/top of glyph
        int64_t             Advance;    // Horizontal offset to advance to next glyph
    };

    class Font {
    public:
        Font();
        ~Font();

        std::string& Name();
        std::map<uint8_t, Character>& Characters();

    private:
        std::string                    m_Name;
        std::map<uint8_t, Character>   m_Characters;
    };
    typedef std::shared_ptr<Font>   FontPtr;
}


#endif //VDGE_FONT_HPP

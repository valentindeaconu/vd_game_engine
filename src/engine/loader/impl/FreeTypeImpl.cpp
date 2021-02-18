//
// Created by Vali on 2/17/2021.
//

#include "FreeTypeImpl.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace vd::loader::impl {

    void FreeTypeImpl::Load(const std::string& path, uint32_t fontSize, vd::model::FontPtr& output) {
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            throw RuntimeError("could not init FreeType Library");
        }

        FT_Face face;
        if (FT_New_Face(ft, path.c_str(), 0, &face)) {
            throw RuntimeError("failed to load font");
        }

        // Set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, fontSize);

        // TODO: Do something with this (should not be here)
        // Disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Then for the first 128 ASCII characters, pre-load/compile their characters and store them
        for (GLubyte c = 0; c < 128; c++) {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                throw RuntimeError("failed to load glyph");
            }

            // Rotate buffer
            uint8_t* buffer = face->glyph->bitmap.buffer;
            for (size_t in = 0, iv = face->glyph->bitmap.rows - 1; in < (face->glyph->bitmap.rows >> 1); ++in, --iv) {
                for (size_t jn = 0, jv = 0; jn < face->glyph->bitmap.width; ++jn, ++jv) {
                    const size_t cn = in * face->glyph->bitmap.width + jn;
                    const size_t cv = iv * face->glyph->bitmap.width + jv;
                    std::swap(buffer[cn], buffer[cv]);
                }
            }

            gl::Texture2DPtr texture = service::TextureService::UncachedCreate(
                Dimension(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                gl::TextureFormat::eR,
                gl::TextureFormat::eR,
                gl::DataType::eUnsignedByte,
                face->glyph->bitmap.buffer
            );

            texture->Bind();
            texture->BilinearFilter();
            texture->WrapClampToBorder();
            texture->Unbind();

            model::Character ch = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };

            output->Characters()[c] = ch;
        }

        // Destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }
}

//
// Created by Vali on 2/17/2021.
//

#include "FontLoader.hpp"

namespace vd::loader {

    void FontLoader::Load(const std::string& path, uint32_t fontSize, model::FontPtr& output) {
        impl::IFontLoaderPtr impl = ObjectOfType<impl::IFontLoader>::Find();
        impl->Load(path, fontSize, output);
    }

    model::FontPtr FontLoader::Load(const std::string& path, uint32_t fontSize) {
        model::FontPtr pFont = std::make_shared<model::Font>();
        Load(path, fontSize, pFont);
        return pFont;
    }

}

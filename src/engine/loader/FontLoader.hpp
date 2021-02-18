//
// Created by Vali on 2/17/2021.
//

#ifndef VDGE_FONTLOADER_HPP
#define VDGE_FONTLOADER_HPP

#include <string>
#include <vector>

#include <engine/model/Font.hpp>
#include <engine/loader/FileLoader.hpp>
#include <engine/service/TextureService.hpp>

namespace vd::loader {
    namespace impl {
        class IFontLoader {
        public:
            virtual void Load(const std::string& path, uint32_t fontSize, model::FontPtr& output) = 0;
        };
        typedef std::shared_ptr<IFontLoader> IFontLoaderPtr;
    }

    class FontLoader {
    public:
        static void Load(const std::string& path, uint32_t fontSize, model::FontPtr& output);
        static model::FontPtr Load(const std::string& path, uint32_t fontSize);
    };
}


#endif //VDGE_FONTLOADER_HPP

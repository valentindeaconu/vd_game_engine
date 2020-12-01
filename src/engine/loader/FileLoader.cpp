//
// Created by Vali on 11/14/2020.
//

#include "FileLoader.hpp"

namespace vd::loader {

    std::string FileLoader::Load(const std::string& path) {
        impl::IFileLoaderPtr impl = ObjectOfType<impl::IFileLoader>::Find();

        std::string content;
        impl->Load(path, content);
        return content;
    }

    void FileLoader::Load(const std::string& path, std::string& output) {
        impl::IFileLoaderPtr impl = ObjectOfType<impl::IFileLoader>::Find();

        impl->Load(path, output);
    }

    void FileLoader::Load(const std::string& path, std::vector<std::string>& lines) {
        impl::IFileLoaderPtr impl = ObjectOfType<impl::IFileLoader>::Find();

        std::string content;
        impl->Load(path, content);

        lines.clear();
        lines.shrink_to_fit();

        boost::split(lines, content, [](char c) { return c == '\n'; });
    }

}
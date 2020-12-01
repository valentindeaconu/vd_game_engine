//
// Created by Vali on 11/14/2020.
//

#include "StreamImpl.hpp"

namespace vd::loader::impl {

    void StreamImpl::Load(const std::string& path, std::string& output) {
        std::ifstream in(path);

        if (!in.is_open()) {
            throw NoSuchFileOrDirectory(path);
        }

        output.clear();
        output.shrink_to_fit();

        in.seekg(0, std::ios::end);
        output.reserve(in.tellg());
        in.seekg(0, std::ios::beg);

        output.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());

        in.close();
    }
}
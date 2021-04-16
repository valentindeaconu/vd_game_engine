//
// Created by Vali on 11/13/2020.
//

#include "ObjectLoader.hpp"

namespace vd::loader {

    vd::model::MeshPtrVec ObjectLoader::Load(const std::string& path) {
        impl::IObjectLoaderPtr impl = vd::ObjectOfType<impl::IObjectLoader>::Find();

        return impl->Load(path);
    }

}
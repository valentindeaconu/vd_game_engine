//
// Created by Vali on 11/13/2020.
//

#ifndef VD_GAME_ENGINE_OBJECTLOADER_HPP
#define VD_GAME_ENGINE_OBJECTLOADER_HPP

#include <string>

#include <engine/model/Mesh.hpp>

#include <engine/injector/ObjectOfType.hpp>

namespace vd::loader {
    namespace impl {
        class IObjectLoader {
        public:
            virtual vd::model::Mesh3DPtrVec Load(const std::string& path) = 0;
        };
        typedef std::shared_ptr<IObjectLoader>  IObjectLoaderPtr;
    }

    class ObjectLoader {
    public:
        static vd::model::Mesh3DPtrVec Load(const std::string& path);
    };
}


#endif //VD_GAME_ENGINE_OBJECTLOADER_HPP

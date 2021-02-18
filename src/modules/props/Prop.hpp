//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_PROP_HPP
#define VD_GAME_ENGINE_PROP_HPP

#include <engine/object/Entity3D.hpp>

#include <engine/loader/ObjectLoader.hpp>

namespace mod::props {
    class Prop : public vd::object::Entity3D {
    public:
        Prop(std::string  path, std::string  file);

        void Setup() override;

        void Update() override;
    private:
        const std::string m_kPath;
        const std::string m_kFile;
    };
    typedef std::shared_ptr<Prop>	PropPtr;
}

#endif //VD_GAME_ENGINE_PROP_HPP
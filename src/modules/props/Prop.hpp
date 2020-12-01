//
// Created by Vali on 11/18/2020.
//

#ifndef VD_GAME_ENGINE_PROP_HPP
#define VD_GAME_ENGINE_PROP_HPP

#include <engine/object/Entity.hpp>

#include <engine/injector/ObjectOfType.hpp>
#include <engine/loader/ObjectLoader.hpp>

namespace mod::props {
    class Prop : public vd::object::Entity {
    public:
        Prop(std::string path, std::string file);
        ~Prop();

        void Init() override;
        void Update() override;
        void CleanUp() override;
    private:
        const std::string m_kPath;
        const std::string m_kFile;
    };
    typedef std::shared_ptr<Prop>	PropPtr;
}

#endif //VD_GAME_ENGINE_PROP_HPP
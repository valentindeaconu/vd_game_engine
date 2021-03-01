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
        struct Details {
            std::string Path;
            std::string File;
            float       Distance;
            bool        Billboard;
        };

        explicit Prop(std::vector<Details> details);

        void Setup() override;
        void Update() override;

        bool BillboardAtLevel(size_t level);
    private:
        const std::vector<Details>  m_kDetails;
    };
    typedef std::shared_ptr<Prop>	PropPtr;
}

#endif //VD_GAME_ENGINE_PROP_HPP
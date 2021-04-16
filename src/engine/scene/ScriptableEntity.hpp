//
// Created by vali on 4/15/21.
//

#ifndef VDGE_SCRIPTABLEENTITY_HPP
#define VDGE_SCRIPTABLEENTITY_HPP

#include "Scene.hpp"

#include "Entity.hpp"

namespace vd::scene {
    class ScriptableEntity {
    public:
        template <typename T>
        T& GetComponent() {
            return m_Entity.template GetComponent<T>();
        }

    protected:
        virtual void OnCreate();
        virtual void OnUpdate(Timestep ts);
        virtual void OnDestroy();

    private:
        Entity m_Entity;
        friend class Scene;
    };
}

#endif //VDGE_SCRIPTABLEENTITY_HPP

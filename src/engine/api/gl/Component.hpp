//
// Created by Vali on 3/17/2021.
//

#ifndef VDGE_GL_COMPONENT_HPP
#define VDGE_GL_COMPONENT_HPP

#include <engine/exception/Exceptions.hpp>

namespace vd::gl {

    class Component {
    public:
        Component();

        void Create();
        void CleanUp();
       
    protected:
        void PassIfCreated() const;
        void PassIfNotCreated() const;

    private:
        virtual void OnCreate() = 0;
        virtual void OnCleanUp() = 0;

        bool m_Created;
    };

}

#endif //VDGE_GLCOMPONENT_HPP

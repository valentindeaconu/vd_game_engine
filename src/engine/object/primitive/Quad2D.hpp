//
// Created by Vali on 2/26/2021.
//

#ifndef VDGE_QUAD2D_HPP
#define VDGE_QUAD2D_HPP

#include <memory>

#include <engine/object/Entity2D.hpp>

namespace vd::object::primitive {
    class Quad2D : public Entity2D {
    public:
        virtual void Setup();
        virtual void Update();
    };
    typedef std::shared_ptr<Quad2D> Quad2DPtr;
}


#endif //VDGE_QUAD2D_HPP

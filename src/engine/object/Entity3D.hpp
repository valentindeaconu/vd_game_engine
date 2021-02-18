//
// Created by Vali on 2/17/2021.
//

#ifndef VDGE_ENTITY3D_HPP
#define VDGE_ENTITY3D_HPP

#include <engine/math/Bounds.hpp>
#include <engine/model/Mesh.hpp>

#include "Entity.hpp"

namespace vd::object {
    class Entity3D : public Entity {
    public:
        virtual void Setup() = 0;

        virtual void Init();
        virtual void CleanUp();

        model::Mesh3DPtrVec& Meshes();
        math::Bounds3Vec& BoundingBoxes();
    private:
        model::Mesh3DPtrVec m_Meshes;
        math::Bounds3Vec    m_BoundingBoxes;
    };
    typedef std::shared_ptr<Entity3D>   Entity3DPtr;
}

#endif //VDGE_ENTITY3D_HPP

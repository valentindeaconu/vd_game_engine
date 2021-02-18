//
// Created by Vali on 2/17/2021.
//

#ifndef VDGE_ENTITY2D_HPP
#define VDGE_ENTITY2D_HPP

#include <engine/math/Bounds.hpp>
#include <engine/model/Mesh.hpp>

#include "Entity.hpp"

namespace vd::object {
    class Entity2D : public Entity {
    public:
        virtual void Setup() = 0;

        virtual void Init();
        virtual void CleanUp();

        model::Mesh2DPtrVec& Meshes();
        math::Bounds2Vec& BoundingBoxes();
    private:
        model::Mesh2DPtrVec m_Meshes;
        math::Bounds2Vec    m_BoundingBoxes;
    };
    typedef std::shared_ptr<Entity2D>   Entity2DPtr;
}

#endif //VDGE_ENTITY2D_HPP

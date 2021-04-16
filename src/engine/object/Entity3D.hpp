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
        Entity3D();

        virtual void Setup() = 0;

        virtual void Init();
        virtual void CleanUp();

        uint64_t LevelOfDetailAtDistance(float distance);

        [[nodiscard]] const model::MeshPtrVec&      Meshes(uint64_t index) const;
        [[nodiscard]] const math::Bounds3Vec&       BoundingBoxes(uint64_t index) const;
        [[nodiscard]] const std::vector<uint64_t>&  BufferIndices(uint64_t index) const;

        void UpdateBoundsForLevel(uint64_t levelOfDetail);
    protected:
        void PushMesh(const model::MeshPtrVec& meshes, float distance);

    private:
        uint64_t m_Count;

        /// Structure of Arrays (SoA) for easy computations
        struct {
            std::vector<float>                  Distances;
            std::vector<model::MeshPtrVec>      Meshes;
            std::vector<math::Bounds3Vec>       BoundingBoxes;
            std::vector<std::vector<uint64_t>>  BufferIndices;
        } m_DetailedMeshes;

    };
    typedef std::shared_ptr<Entity3D>   Entity3DPtr;
}

#endif //VDGE_ENTITY3D_HPP

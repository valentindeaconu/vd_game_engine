#include "Prop.hpp"

namespace mod::props {
    Prop::Prop(std::vector<Details> details)
        : m_kDetails(std::move(details))
    {
    }

    void Prop::Setup() {
        for (const auto& lodDetails : m_kDetails) {
            // TODO: Solve for cross-platform paths
            auto meshes = vd::loader::ObjectLoader::Load(lodDetails.Path + '/' + lodDetails.File);

            if (lodDetails.Billboard) {
                for (auto& mesh : meshes) {
                    for (auto& material : mesh->Materials()) {
                        material.DiffuseMap()->Bind();
                        material.DiffuseMap()->LinearFilter();
                        material.DiffuseMap()->Unbind();
                    }
                }
            }

            this->PushMesh(meshes, lodDetails.Distance);
        }
    }

    void Prop::Update() { }

    bool Prop::BillboardAtLevel(size_t level) {
        return m_kDetails[level].Billboard;
    }
}
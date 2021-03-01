#include "Prop.hpp"

namespace mod::props {
    Prop::Prop(std::vector<Details> details)
        : m_kDetails(std::move(details))
    {
    }

    void Prop::Setup() {
        for (const auto& lodDetails : m_kDetails) {
            // TODO: Solve for cross-platform paths
            this->PushMesh(vd::loader::ObjectLoader::Load(lodDetails.Path + '/' + lodDetails.File),
                           lodDetails.Distance);
        }
    }

    void Prop::Update() { }

    bool Prop::BillboardAtLevel(size_t level) {
        return m_kDetails[level].Billboard;
    }
}
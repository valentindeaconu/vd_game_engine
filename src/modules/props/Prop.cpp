#include "Prop.hpp"

namespace mod::props {
    Prop::Prop(std::string path, std::string file)
        : m_kPath(std::move(path))
        , m_kFile(std::move(file))
    {
    }

    void Prop::Setup() {
        // TODO: Solve for cross-platform paths
        Meshes() = vd::loader::ObjectLoader::Load(m_kPath + '/' + m_kFile);
    }

    void Prop::Update() { }
}
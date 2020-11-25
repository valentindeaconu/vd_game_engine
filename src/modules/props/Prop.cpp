#include "Prop.hpp"

#include <utility>

namespace mod::props {
    Prop::Prop(std::string path, std::string file)
        : m_kPath(std::move(path))
        , m_kFile(std::move(file))
    {
    }

    Prop::~Prop() = default;

    void Prop::Init() {
        this->Meshes() = vd::loader::ObjectLoader::Load(m_kPath + '/' + m_kFile);

        Entity::Init();
    }

    void Prop::Update() { }

    void Prop::CleanUp() {
        Entity::CleanUp();
    }
}
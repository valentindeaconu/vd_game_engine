#include "StaticObject.hpp"

#include <utility>

namespace mod::sobj {
    StaticObject::StaticObject(std::string path, std::string objFile)
        : path(std::move(path))
        , objFile(std::move(objFile))
    {
    }

    StaticObject::~StaticObject() = default;

    void StaticObject::Init() {
        this->Meshes() = vd::loader::ObjectLoader::Load(path + '/' + objFile);

        Entity::Init(); // call super.Init() to initialize meshBuffers;
    }

    void StaticObject::Update() {

    }

    void StaticObject::CleanUp() {
        Entity::CleanUp();
    }
}
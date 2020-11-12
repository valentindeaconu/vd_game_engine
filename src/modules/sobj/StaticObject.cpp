#include "StaticObject.hpp"

namespace mod::sobj
{
    StaticObject::StaticObject(const std::string& path, const std::string& objFile)
        : path(path)
        , objFile(objFile)
    {
    }

    StaticObject::~StaticObject() = default;

    void StaticObject::Init() {
        vd::objloader::OBJLoader objLoader;

        vd::model::MeshPtrVec& meshPtrVec = Meshes();
        objLoader.load(path, objFile, meshPtrVec);

        Entity::Init(); // call super.Init() to initialize meshBuffers;
    }

    void StaticObject::Update() {

    }

    void StaticObject::CleanUp() {
        Entity::CleanUp();
    }
}
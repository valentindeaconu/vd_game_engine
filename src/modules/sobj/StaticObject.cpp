#include "StaticObject.hpp"

namespace mod::sobj
{
    StaticObject::StaticObject(const std::string& path, const std::string& objFile)
        : Entity(nullptr)
        , path(path)
        , objFile(objFile)
    {
    }

    StaticObject::~StaticObject() = default;

    void StaticObject::init() {
        vd::objloader::OBJLoader objLoader;

        vd::model::MeshPtrVec& meshPtrVec = getMeshes();
        objLoader.load(path, objFile, meshPtrVec);

        Entity::init(); // call super.init() to initialize meshBuffers;
    }

    void StaticObject::update() {

    }

    void StaticObject::cleanUp() {
        Entity::cleanUp();
    }
}
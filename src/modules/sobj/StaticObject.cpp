#include "StaticObject.hpp"

namespace mod
{
	namespace sobj
	{
		StaticObject::StaticObject(const std::string& path, const std::string& objFile)
			: Entity(nullptr)
			, path(path)
			, objFile(objFile)
		{
		}

		StaticObject::~StaticObject()
		{
		}

		void StaticObject::init()
		{
			vd::objloader::OBJLoader objLoader;

			vd::model::MeshPtrVec& meshPtrVec = getMeshes();
			objLoader.load(path, objFile, meshPtrVec);

			Entity::init(); // call super.init() to initialize meshBuffers;
		}

		void StaticObject::update()
		{
			// no update for this kind of objects
		}
		
		void StaticObject::cleanUp()
		{
			Entity::cleanUp();
		}
	}
}
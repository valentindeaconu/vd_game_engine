#ifndef __STATIC_OBJECT_HPP_
#define __STATIC_OBJECT_HPP_

#include <engine/object/Entity.hpp>
#include <engine/foundation/objloader/OBJLoader.hpp>

namespace mod::sobj {
    class StaticObject : public vd::object::Entity {
    public:
        StaticObject(const std::string& path, const std::string& objFile);
        ~StaticObject();

        void Init() override;
        void Update() override;
        void CleanUp() override;
    private:
        std::string path;
        std::string objFile;
    };
    typedef std::shared_ptr<StaticObject>	StaticObjectPtr;
}

#endif // !__STATIC_OBJECT_HPP_
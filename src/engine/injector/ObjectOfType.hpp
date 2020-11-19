//
// Created by Vali on 10/15/2020.
//

#ifndef VD_GAME_ENGINE_OBJECTOFTYPE_HPP
#define VD_GAME_ENGINE_OBJECTOFTYPE_HPP

#include <engine/exception/Exceptions.hpp>

#include <unordered_map>
#include <memory>

namespace vd {
    template <typename T>
    class ObjectOfType {
    public:
        static const std::shared_ptr<T>& Find() {
            if (m_Instance == nullptr)
                throw MissingDependency(typeid(T).name());
            return m_Instance;
        }
        static void Provide(const std::shared_ptr<T>& object) { m_Instance = object; }
    private:
        static inline std::shared_ptr<T> m_Instance;
    };
}


#endif //VD_GAME_ENGINE_OBJECTOFTYPE_HPP

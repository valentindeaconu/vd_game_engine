//
// Created by Vali on 10/15/2020.
//

#ifndef VD_GAME_ENGINE_OBJECTMANAGER_HPP
#define VD_GAME_ENGINE_OBJECTMANAGER_HPP

#include <unordered_map>
#include <memory>

namespace vd::core {
    template <typename T>
    class FindObjectOfType {
    public:
        static const std::shared_ptr<T>& Get() { return m_Instance; }
        static void Provide(const std::shared_ptr<T>& object) { m_Instance = object; }
    private:
        static inline std::shared_ptr<T> m_Instance;
    };
}


#endif //VD_GAME_ENGINE_OBJECTMANAGER_HPP

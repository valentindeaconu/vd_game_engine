//
// Created by Vali on 11/12/2020.
//

#ifndef VD_GAME_ENGINE_OBSERVER_HPP
#define VD_GAME_ENGINE_OBSERVER_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <stdexcept>

namespace vd::datastruct {
    class Observer {
    public:
        typedef std::unordered_map<std::string, std::string> params_t;
        inline static const params_t kEmptyParams = params_t();

        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void Render(const params_t& params) = 0;
        virtual void CleanUp() = 0;
    };
    typedef std::shared_ptr<Observer>	ObserverPtr;

    class Observable {
    public:
        typedef uint64_t    priority_t;

        enum BroadcastType {
            eInit = 0,
            eUpdate,
            eRender,
            eCleanUp
        };

        void Subscribe(const ObserverPtr& observer, priority_t priority = kHalfPriority);
        void Unsubscribe(const ObserverPtr& observer);

        void Broadcast(const BroadcastType& broadcastType, const Observer::params_t& params = Observer::kEmptyParams) const;
    private:
        static const priority_t kHalfPriority = (1ULL << 32);

        struct Member {
            priority_t priority;
            ObserverPtr observer;
        };

        std::list<Member> m_Observers;
    };
}


#endif //VD_GAME_ENGINE_OBSERVER_HPP

//
// Created by Vali on 11/12/2020.
//

#include "Observer.hpp"

namespace vd::datastruct {

    void Observable::Subscribe(const ObserverPtr& observer, uint64_t priority) {
        Member member = { .priority = priority, .observer = observer };

        if (m_Observers.empty() || m_Observers.front().priority > priority) {
            m_Observers.push_front(member);
        } else if (m_Observers.back().priority <= member.priority) {
            m_Observers.push_back(member);
        } else {
            auto it = m_Observers.begin();
            for (; it != m_Observers.end(); ++it) {
                if (it->priority > priority) {
                    break;
                }
            }

            if (it == m_Observers.end()) {
                throw std::runtime_error("could not insert new member, unknown state");
            }

            m_Observers.insert(it, std::move(member));
        }
    }

    void Observable::Unsubscribe(const ObserverPtr& observer) {
        auto it = std::find_if(m_Observers.begin(), m_Observers.end(), [&](const Member& x) {
            return x.observer.get() == observer.get();
        });

        if (it != m_Observers.end()) {
            m_Observers.erase(it);
        }
    }

    void Observable::Broadcast(const BroadcastType& broadcastType, const Observer::params_t& params) const {
        switch (broadcastType) {
            case eInit: {
                for (auto& member : m_Observers) {
                    member.observer->Init();
                }
                break;
            }
            case eUpdate: {
                for (auto& member : m_Observers) {
                    member.observer->Update();
                }
                break;
            }
            case eRender: {
                for (auto& member : m_Observers) {
                    try {
                        member.observer->Render(params);
                    } catch (std::runtime_error& e) {
                        continue;
                    }
                }
                break;
            }
            case eCleanUp: {
                for (auto& member : m_Observers) {
                    member.observer->CleanUp();
                }
                break;
            }
        }
    }
}
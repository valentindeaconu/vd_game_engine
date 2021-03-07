//
// Created by Vali on 3/2/2021.
//

#ifndef VDGE_TIMEMANAGER_HPP
#define VDGE_TIMEMANAGER_HPP

#include <memory>

#include <engine/component/IManager.hpp>

#include <engine/property/Properties.hpp>
#include <engine/loader/PropertiesLoader.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/context/Context.hpp>

#include "Time.hpp"

namespace vd::time {
    class TimeManager : public vd::component::IManager, public vd::injector::Injectable {
    public:
        explicit TimeManager(const std::string& propsFilePath);

        void Link() override;

        void Init() override;
        void Update() override;
        void CleanUp() override;

        [[nodiscard]] bool AM() const;
        [[nodiscard]] const float& CurrentAngle() const;
        [[nodiscard]] const TimePtr& CurrentTime() const;
    private:
        float               m_Speed;
        float               m_CurrentAngle;
        TimePtr             m_pTime;

        context::ContextPtr m_pContext;
    };
    typedef std::shared_ptr<TimeManager>    TimeManagerPtr;
}


#endif //VDGE_TIMEMANAGER_HPP

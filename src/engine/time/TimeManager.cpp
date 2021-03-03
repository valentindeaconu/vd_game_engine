//
// Created by Vali on 3/2/2021.
//

#include "TimeManager.hpp"

namespace vd::time {

    TimeManager::TimeManager(const std::string& propsFilePath) {
        auto pProperties = vd::loader::PropertiesLoader::Load(propsFilePath);

        uint8_t initHour = pProperties->Get<int>("Init.Hour");
        uint8_t initMinute = pProperties->Get<int>("Init.Minute");

        m_pTime = std::make_shared<Time>(initHour, initMinute);
        if (!m_pTime->Valid()) {
            throw exception::TimeError("invalid initial time");
        }

        // How many minutes each second
        auto speed = pProperties->Get<float>("Time.Speed");
        // An hour has 30 degrees, a minute is 1/60 of an hour so 0.5 degrees for each minute
        m_Speed = 0.5f * speed;

        m_CurrentAngle = m_pTime->ToAngle();

        /*if ((initHour == 0 || initHour == 12) && initMinute == 0) {
            m_CurrentAngle = .0f;
        } else {
            m_CurrentAngle = float(initHour) * 30.0f + float(initMinute) * 0.5f;
        }*/

        if (m_CurrentAngle < 0.0f || m_CurrentAngle >= 360.0f) {
            throw exception::TimeError("invalid computed angle");
        }
    }

    void TimeManager::Link() {
        m_pContext = vd::ObjectOfType<context::Context>::Find();
    }

    void TimeManager::Init() { }

    void TimeManager::Update() {
        m_CurrentAngle += m_Speed * m_pContext->FrameTime();
        m_CurrentAngle = (m_CurrentAngle >= 360.0f) ? (m_CurrentAngle - 360.0f) : m_CurrentAngle;

        Time aux(m_CurrentAngle);

        if ((!m_pTime->AM() && m_pTime->Hour() == 11) && aux.Hour() == 0) {
            // from 23 to 0
            m_pTime->Hour(0);
        } else if ((m_pTime->AM() && m_pTime->Hour() == 11) && aux.Hour() == 0) {
            // from 11 to 12
            m_pTime->Hour(12);
        } else if (!m_pTime->AM()) {
            m_pTime->Hour(aux.Hour() + 12);
        } else {
            m_pTime->Hour(aux.Hour());
        }

        m_pTime->Minute(aux.Minute());
    }

    void TimeManager::CleanUp() { }

    bool TimeManager::AM() const {
        return m_pTime->AM();
    }

    const float& TimeManager::CurrentAngle() const {
        return m_CurrentAngle;
    }

    const TimePtr& TimeManager::CurrentTime() const {
        return m_pTime;
    }
}

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

        // Compute current angle
        m_AM = (initHour < 12);

        if ((initHour == 0 || initHour == 12) && initMinute == 0) {
            m_CurrentAngle = .0f;
        } else {
            m_CurrentAngle = float(initHour) * 30.0f + float(initMinute) * 0.5f;
        }

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

        uint8_t hour = 0;
        uint8_t minute = 0;
        float angle = m_CurrentAngle;
        while (angle >= 30.0f) {
            hour++;
            angle -= 30.0f;
        }

        if (angle > 0.0f) {
            while (angle >= 0.5f) {
                minute++;
                angle -= 0.5f;
            }
        }

        if (m_pTime->Hour() == 23 && hour == 0) {
            m_AM = true;
        } else if (m_pTime->Hour() == 11 && hour == 0) {
            m_AM = false;
        }

        m_pTime->Hour() = hour + (m_AM ? 0 : 12);
        m_pTime->Minute() = minute;
    }

    void TimeManager::CleanUp() { }

    bool TimeManager::AM() const {
        return m_AM;
    }

    const float& TimeManager::CurrentAngle() const {
        return m_CurrentAngle;
    }

    const TimePtr& TimeManager::CurrentTime() const {
        return m_pTime;
    }
}

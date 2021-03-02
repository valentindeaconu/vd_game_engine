//
// Created by Vali on 3/2/2021.
//

#include "Time.hpp"

namespace vd::time {
    namespace exception {
        TimeError::TimeError(const std::string& message)
                : CustomException("TimeError", message)
        {
        }
    }

    Time::Time()
        : m_Hour(0)
        , m_Minute(0)
    {
    }

    Time::Time(uint8_t hour, uint8_t minute)
        : m_Hour(hour)
        , m_Minute(minute)
    {
    }

    uint8_t& Time::Minute() {
        return m_Minute;
    }

    uint8_t& Time::Hour() {
        return m_Hour;
    }

    bool Time::Valid() const {
        return m_Hour < 24 && m_Minute < 60;
    }

    std::string Time::ToString() const {
        std::stringstream ss;
        ss << ((m_Hour < 10) ? "0" : "") << int(m_Hour) << ":" << ((m_Minute < 10) ? "0" : "") << int(m_Minute);
        return ss.str();
    }

}
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

    Time::Time(uint8_t hour, uint8_t minute)
        : m_Minute(minute)
    {
        if (hour < 12) {
            m_Hour = hour;
            m_AM = true;
        } else {
            m_Hour = hour - 12;
            m_AM = false;
        }
    }

    Time::Time(float angle, bool AM)
        : m_AM(AM)
    {
        // An hour has 30 degrees, so dividing the angle with 30 should give us the hour (1/30 = 0.03333)
        m_Hour = hour_t(glm::floor(angle * 0.03333333334f));

        // Now we subtract the hours from the angle and the rest should represent minutes
        float rest = angle - (float(m_Hour) * 30.0f);
        // A minute is 1/60 of an hour so a minute represents 0.5 degrees
        // Dividing the rest by 0.5 should give us the minutes (1/0.5 = 2.0)
        m_Minute = hour_t(glm::floor(rest * 2.0f));

        if (!Valid()) {
            throw exception::TimeError("invalid time");
        }
    }

    bool Time::operator==(const Time& other) const {
        return m_AM == other.m_AM && m_Hour == other.m_Hour && m_Minute == other.m_Minute;
    }

    bool Time::operator>(const Time& other) const {
        return (m_Hour > other.m_Hour) || (m_Minute > other.m_Minute);
    }

    bool Time::operator<(const Time& other) const {
        return (m_Hour < other.m_Hour) || (m_Minute < other.m_Minute);
    }

    bool Time::operator>=(const Time& other) const {
        return (m_Hour >= other.m_Hour) || (m_Minute >= other.m_Minute);
    }

    bool Time::operator<=(const Time& other) const {
        return (m_Hour <= other.m_Hour) || (m_Minute <= other.m_Minute);
    }

    bool Time::Valid() const {
        return m_Hour < 12 && m_Minute < 60;
    }

    float Time::ToAngle() const {
        return float(m_Hour) * 30.0f + float(m_Minute) * 0.5f;
    }

    bool Time::AM() const {
        return m_AM;
    }

    void Time::Hour(hour_t h) {
        if (h >= 24) {
            throw exception::TimeError("invalid hour to set");
        }

        if (h >= 12) {
            m_AM = false;
            h -= 12;
        } else {
            m_AM = true;
        }

        m_Hour = h;
    }

    [[nodiscard]] hour_t Time::Hour() const {
        return m_Hour;
    }

    void Time::Minute(minute_t m) {
        if (m >= 60) {
            throw exception::TimeError("invalid minute to set");
        }

        m_Minute = m;
    }

    [[nodiscard]] minute_t Time::Minute() const {
        return m_Minute;
    }


    std::string Time::ToString() const {
        hour_t h = (m_AM) ? m_Hour : (m_Hour + 12);

        std::stringstream ss;
        ss << ((h < 10) ? "0" : "") << int(h) << ":" << ((m_Minute < 10) ? "0" : "") << int(m_Minute);
        return ss.str();
    }

}
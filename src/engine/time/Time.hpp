//
// Created by Vali on 3/2/2021.
//

#ifndef VDGE_TIME_HPP
#define VDGE_TIME_HPP

#include <memory>
#include <sstream>
#include <glm/glm.hpp>

#include <engine/exception/Exceptions.hpp>

namespace vd::time {
    typedef uint8_t hour_t;
    typedef uint8_t minute_t;

    namespace exception {
        struct TimeError : public CustomException {
            explicit TimeError(const std::string& message);
        };
   }

    class Time {
    public:
        explicit Time(uint8_t hour = 0, uint8_t minute = 0);
        explicit Time(float angle, bool AM = true);

        bool operator==(const Time& other) const;
        bool operator>(const Time& other) const;
        bool operator<(const Time& other) const;
        bool operator>=(const Time& other) const;
        bool operator<=(const Time& other) const;

        [[nodiscard]] bool  Valid() const;
        [[nodiscard]] float ToAngle() const;
        [[nodiscard]] bool  AM() const;

        void Hour(hour_t h);
        [[nodiscard]] hour_t Hour() const;
        void Minute(minute_t m);
        [[nodiscard]] minute_t Minute() const;

        [[nodiscard]] std::string ToString() const;
    private:
        bool        m_AM;
        hour_t      m_Hour;
        minute_t    m_Minute;
    };
    typedef std::shared_ptr<Time>   TimePtr;
}


#endif //VDGE_TIME_HPP

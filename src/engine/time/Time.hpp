//
// Created by Vali on 3/2/2021.
//

#ifndef VDGE_TIME_HPP
#define VDGE_TIME_HPP

#include <memory>
#include <sstream>

#include <engine/exception/Exceptions.hpp>

namespace vd::time {
    namespace exception {
        struct TimeError : public CustomException {
            explicit TimeError(const std::string& message);
        };
   }

    class Time {
    public:
        Time();
        Time(uint8_t hour, uint8_t minute);

        [[nodiscard]] bool Valid() const;

        uint8_t& Hour();
        uint8_t& Minute();

        [[nodiscard]] std::string ToString() const;
    private:
        uint8_t m_Hour;
        uint8_t m_Minute;
    };
    typedef std::shared_ptr<Time>   TimePtr;
}


#endif //VDGE_TIME_HPP

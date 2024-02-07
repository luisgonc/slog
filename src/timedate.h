//
// Created by lcrgo on 29/01/2024.
//

#ifndef SMALL_LOG_TIMEDATE_H
#define SMALL_LOG_TIMEDATE_H

#include <cstdint>

namespace slog {

    class timedate {
    public:
        timedate();
        virtual ~timedate();

        const uint16_t &getMYear() const;
        bool setMYear(const uint16_t &mYear);

        const uint8_t &getMMonth() const;
        bool setMMonth(const uint8_t &mMonth);

        const uint8_t &getMDay() const;
        bool setMDay(const uint8_t &mDay);

        const uint8_t &getMHour() const;
        bool setMHour(const uint8_t &mHour);

        const uint8_t &getMMinute() const;
        bool setMMinute(const uint8_t &mMinute);

        const uint8_t &getMSecond() const;
        bool setMSecond(const uint8_t &mSecond);

        const uint16_t &getMMillisecond() const;
        bool setMMillisecond(const uint16_t &mMillisecond);

    private:
        uint16_t m_year;
        uint8_t m_month;
        uint8_t m_day;
        uint8_t m_hour;
        uint8_t m_minute;
        uint8_t m_second;
        uint16_t m_millisecond;
    };

} // slog

#endif //SMALL_LOG_TIMEDATE_H

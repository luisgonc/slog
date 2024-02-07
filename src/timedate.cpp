//
// Created by lcrgo on 29/01/2024.
//

#include "timedate.h"

namespace slog {
    timedate::timedate() :
    m_year(2000), m_month(1), m_day(1), m_hour(0), m_minute(0), m_second(0), m_millisecond(0) {}

    timedate::~timedate() {}

    const uint16_t &timedate::getMYear() const {
        return m_year;
    }

    bool timedate::setMYear(const uint16_t &mYear) {
        if(mYear <= 9999) {
            m_year = mYear;
            return true;
        }
        return false;
    }

    const uint8_t &timedate::getMMonth() const {
        return m_month;
    }

    bool timedate::setMMonth(const uint8_t &mMonth) {
        if(mMonth <= 12 && mMonth > 0) {
            m_month = mMonth;
            return true;
        }
        return false;
    }

    const uint8_t &timedate::getMDay() const {
        return m_day;
    }

    bool timedate::setMDay(const uint8_t &mDay) {
        if(mDay <= 31 && mDay > 0) {
            m_day = mDay;
            return true;
        }
        return false;
    }

    const uint8_t &timedate::getMHour() const {
        return m_hour;
    }

    bool timedate::setMHour(const uint8_t &mHour) {
        if(mHour <= 23) {
            m_hour = mHour;
            return true;
        }
        return false;
    }

    const uint8_t &timedate::getMMinute() const {
        return m_minute;
    }

    bool timedate::setMMinute(const uint8_t &mMinute) {
        if(mMinute <= 59) {
            m_minute = mMinute;
            return true;
        }
        return false;
    }

    const uint8_t &timedate::getMSecond() const {
        return m_second;
    }

    bool timedate::setMSecond(const uint8_t &mSecond) {
        if(mSecond <= 59) {
            m_second = mSecond;
            return true;
        }
        return false;
    }

    const uint16_t &timedate::getMMillisecond() const {
        return m_millisecond;
    }

    bool timedate::setMMillisecond(const uint16_t &mMillisecond) {
        if(mMillisecond <= 999) {
            m_millisecond = mMillisecond;
            return true;
        }
        return false;
    }
} // slog
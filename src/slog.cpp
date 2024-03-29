//
// Created by lcrgo on 28/01/2024.
//

#include "slog.h"

#include <cstdio>

namespace slog {

    logger::logger(const char* logger_name) :
    m_level(level::info),
    m_last_log_level(level::info),
    m_radix(radix::dec),
    m_print_date(false),
    m_time_provider(nullptr) {

        /* Store the given logger name up to the buffer size, the excess will be trimmed */
        std::snprintf(m_logger_name, sizeof(m_logger_name), "%s", logger_name);

        for (int i = 0; i < MAX_NBR_LOG_APPENDER; ++i) {
            m_appenders[i] = nullptr;
        }

        std::memset(m_print_timestamp, 0, sizeof(m_print_timestamp));
        std::memset(m_print_level_str, 0, sizeof(m_print_level_str));
        std::memset(m_print_logger_name, 0, sizeof(m_print_logger_name));
    }

    logger::~logger() {}

    logger::level logger::get_Level() const {
        return m_level;
    }

    void logger::set_Level(level log_level) {
        m_level = log_level;
    }

    bool logger::add_appender(std::function<void(const char*)> appender) {
        for (int i = 0; i < MAX_NBR_LOG_APPENDER; ++i) {
            if (m_appenders[i] == nullptr) {
                m_appenders[i] = appender;
                return true;
            }
        }
        return false;
    }

    void logger::set_time_provider(std::function<timedate()> time_provider) {
        m_time_provider = time_provider;
    }

    const char *logger::get_name() const {
        return m_logger_name;
    }

    void logger::set_print_date(bool print_date) {
        m_print_date = print_date;
    }

    bool logger::get_print_date() {
        return m_print_date;
    }

    const char* logger::get_print_timestamp() {
        /* The timestamp should look like this: [2024/02/10 23:12:35.123] or
         *                           like this: [23:12:35.123]
         * depending if the date should be printed or NOT */

        if (m_time_provider != nullptr) {
            /* Get the current time from the time provider */
            timedate td = m_time_provider();

            if(m_print_date) {
                /* Build the timestamp using the information from the time provider */
                std::snprintf(m_print_timestamp, sizeof(m_print_timestamp), "[%04d/%02d/%02d %02d:%02d:%02d.%03d]",
                              td.getMYear(), td.getMMonth(), td.getMDay(),
                              td.getMHour(), td.getMMinute(), td.getMSecond(), td.getMMillisecond());
            } else {
                /* Build the timestamp using the information from the time provider */
                std::snprintf(m_print_timestamp, sizeof(m_print_timestamp), "[%02d:%02d:%02d.%03d]",
                              td.getMHour(), td.getMMinute(), td.getMSecond(), td.getMMillisecond());
            }
        }

        return m_print_timestamp;
    }

    const char *logger::get_print_level_str(level level) {
        std::memset(m_print_level_str, 0, sizeof(m_print_level_str));

        switch (level) {
            case level::trace: {
                std::snprintf(m_print_level_str, sizeof(m_print_level_str), "[%s]", "TRACE");
                break;
            }
            case level::debug: {
                std::snprintf(m_print_level_str, sizeof(m_print_level_str), "[%s]", "DEBUG" );
                break;
            }
            case level::info: {
                std::snprintf(m_print_level_str, sizeof(m_print_level_str), "[%s]", "INFO " );
                break;
            }
            case level::warn: {
                std::snprintf(m_print_level_str, sizeof(m_print_level_str), "[%s]", "WARN " );
                break;
            }
            case level::error: {
                std::snprintf(m_print_level_str, sizeof(m_print_level_str), "[%s]", "ERROR" );
                break;
            }
            case level::fatal: {
                std::snprintf(m_print_level_str, sizeof(m_print_level_str), "[%s]", "FATAL" );
                break;
            }
            case level::disabled: {
                std::snprintf(m_print_level_str, sizeof(m_print_level_str), "[%s]", "DISAB" );
                break;
            }
            default: {
                std::snprintf(m_print_level_str, sizeof(m_print_level_str), "[%s]", "UNKNW" );
                break;
            }
        }

        return m_print_level_str;
    }

    const char *logger::get_print_logger_name() {

        std::memset(m_print_logger_name, 0, sizeof(m_print_logger_name));

        std::snprintf(m_print_logger_name, sizeof(m_print_logger_name), "[%s]", m_logger_name);

        return m_print_logger_name;
    }

    void logger::log_write(level level, const char *msg) {

        /* check the log level */
        if(m_level == level::disabled || 
           level == level::disabled ||
           level < m_level) {
            return;
        }

        for (int i = 0; i < MAX_NBR_LOG_APPENDER; ++i) {
            if (m_appenders[i] != nullptr) {
                m_appenders[i](msg);
            }
        }
    }

    logger &logger::log(logger::level log_level, const char *msg) {

        log_write(log_level, "\n");
        log_write(log_level, get_print_timestamp());
        log_write(log_level, get_print_level_str(log_level));
        log_write(log_level, get_print_logger_name());
        log_write(log_level, " ");
        log_write(log_level, msg);

        m_last_log_level = log_level;

        return *this;
    }

    logger &logger::log(logger::level log_level, const std::string &msg) {

        log(log_level, msg.c_str());

        return *this;
    }

    logger &logger::log(logger::level log_level, const std::string_view &msg) {

        log(log_level, msg.data());

        return *this;
    }

    logger &logger::operator<<(const char *msg) {

        log_write(m_last_log_level, msg);

        return *this;
    }

    logger &logger::operator<<(const std::string &msg) {

            log_write(m_last_log_level, msg.c_str());

            return *this;
    }

    logger &logger::operator<<(const std::string_view &msg) {

            log_write(m_last_log_level, msg.data());

            return *this;
    }

    logger &logger::operator<<(std::chrono::seconds time) {

        operator<<(radix::dec);
        operator<<(time.count());
        operator<<("s");

        return *this;
    }

    logger &logger::operator<<(std::chrono::milliseconds time) {

        operator<<(radix::dec);
        operator<<(time.count());
        operator<<("ms");

        return *this;
    }

    logger &logger::operator<<(std::chrono::microseconds time) {

        operator<<(radix::dec);
        operator<<(time.count());
        operator<<("us");

        return *this;
    }

    logger &logger::operator<<(logger::radix rdx) {

        m_radix = rdx;

        return *this;
    }


} // slog
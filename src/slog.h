//
// Created by lcrgo on 28/01/2024.
//

#ifndef SMALL_LOG_SLOG_H
#define SMALL_LOG_SLOG_H

#include <string>
#include <cstring>
#include <string_view>
#include <functional>
#include <chrono>

#include "timedate.h"

namespace slog {

#ifndef LOG_NAME_LEN_MAX
#define  MAX_LOG_NAME_LEN 21 /* Max name lengh including null terminator */
#endif

#ifndef MAX_NBR_LOG_APPENDER
#define MAX_NBR_LOG_APPENDER 3
#endif


    class logger {
    public:
        /* log level enumeration */
        enum class level {TRACE=0, DEBUG, INFO, WARN, ERROR, FATAL, DISABLED};

        /* radix enumeration */
        enum class radix {BIN=2, OCT=8, DEC=10, HEX=16};

        /* default constructor */
        explicit logger(const char* logger_name);
        /* default destructor */
        virtual ~logger();
        /* disable copy constructor */
        logger(const logger&) = delete;
        /* disable copy assignment */
        logger& operator=(const logger&) = delete;

        /* public member functions */
        /**
         * @brief Get the logger name
         * @return const char*, logger name
         */
        const char *get_name() const;

        /* getter & setter for Log level */
        /**
         * @brief Get the current log level
         * @return level, log level
         */
        level get_Level() const;
        /**
         * @brief Set the current log level
         * @param log_level, log level
         */
        void set_Level(level log_level);

        /**
         * @brief Add appender to logger, appenders are functions written by user to handle log
         *        messages produced by the logger. Usually appenders are used to write log messages
         *        to the console or to a file.
         * @param appender, function pointer to appender which will be called when log is written
         * @return true if appender is added successfully, false otherwise
         */
        bool add_appender(std::function<void(const char*)> appender);

        /**
         * @brief Set time provider, time provider is a function written by user to provide the
         *        current time when log is written. This is useful when you want to use a custom
         *        time provider, for example when you want to use a real time clock or a time
         *        server to provide the time.
         * @param time_provider, function pointer to time provider
         */
        void set_time_provider(std::function<timedate()> time_provider);

        /**
         * @brief Set the print date flag, when this flag is set to true the logger will print the
         *       date in the log message. The date is provided by the time provider.
         * @param print_date, true to print the date, false otherwise
         */
        void set_print_date(bool print_date);

        /**
         * @brief Get the print date flag
         * @return bool, true if the date is printed, false otherwise
         */
        bool get_print_date();

        logger& log(level log_level, const char* msg);

        logger& log(level log_level, const std::string& msg);

        logger& log(level log_level, const std::string_view& msg);

        logger& operator<<(const char* msg);

        logger& operator<<(const std::string& msg);

        logger& operator<<(const std::string_view& msg);

        logger& operator<<(std::chrono::seconds time);

        logger& operator<<(std::chrono::milliseconds time);

        logger& operator<<(std::chrono::microseconds time);

        logger& operator<<(radix rdx);

        template <typename T, std::enable_if_t<std::is_integral<T>::value, bool> = true>
        logger& operator<<(T value) {
            const char* const digits = "0123456789ABCDEF";
            const unsigned int radix = static_cast<unsigned int>(m_radix);
            static constexpr size_t MaxFieldWidth = 34;
            char fielddata[MaxFieldWidth];
            size_t fieldindex = 0;

            int first = 0;
            switch (m_radix)
            {
                default:
                    /* intentional fall through */
                case radix::DEC:
                    break;
                case radix::BIN:
                    fielddata[0] = '0';
                    fielddata[1] = 'b';
                    fieldindex += 2;
                    first += 2;
                    break;
                case radix::OCT:
                    fielddata[0] = '0';
                    fielddata[1] = 'o';
                    fieldindex += 2;
                    first += 2;
                    break;
                case radix::HEX:
                    fielddata[0] = '0';
                    fielddata[1] = 'x';
                    fieldindex += 2;
                    first += 2;
                    break;
            }

            // calculate digits (they will be reversed)
            if ( value < 0 )
            {
                fielddata[fieldindex] = '-';
                fieldindex += 1;
            }

            T remainder;
            do {
                remainder = value % radix;
                fielddata[fieldindex] = digits[remainder];
                value /= radix;
                fieldindex += 1;
            } while ( value != 0 );

            // reverse order
            int last = fieldindex-1;
            while ( (last-first) > 0 )
            {
                char tmp = fielddata[last];
                fielddata[last] = fielddata[first];
                fielddata[first] = tmp;
                first += 1;
                last -= 1;
            }

            fielddata[fieldindex] = '\0';

            log_write(m_last_log_level, fielddata);

            return *this;
        }

    private:
        /* private member functions */
        const char* get_print_timestamp();
        const char* get_print_level_str(level level);
        const char* get_print_logger_name();
        void log_write(level level, const char *msg);

        /* member variables */
        level m_level;
        level m_last_log_level;
        radix m_radix;
        bool m_print_date;
        std::function<timedate()> m_time_provider;
        std::function<void(const char*)> m_appenders[MAX_NBR_LOG_APPENDER];
        char m_logger_name[MAX_LOG_NAME_LEN];
        char m_print_timestamp[40];
        char m_print_level_str[10];
        char m_print_logger_name[MAX_LOG_NAME_LEN+2];

    };

} // slog

/* Logging Makros */
#define SLOG_TRACE(logger, msg) logger.log(slog::logger::level::TRACE, msg)
#define SLOG_DEBUG(logger, msg) logger.log(slog::logger::level::DEBUG, msg)
#define SLOG_INFO(logger, msg) logger.log(slog::logger::level::INFO, msg)
#define SLOG_WARN(logger, msg) logger.log(slog::logger::level::WARN, msg)
#define SLOG_ERROR(logger, msg) logger.log(slog::logger::level::ERROR, msg)
#define SLOG_FATAL(logger, msg) logger.log(slog::logger::level::FATAL, msg)

#endif //SMALL_LOG_SLOG_H

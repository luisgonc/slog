#include "slog.h"

#include "gtest/gtest.h"

#include <string>
#include <sstream>
#include <chrono>


TEST(SmallLogTest, logger_name) {

    /* The actual log size max lengh is the defined max length - 1 trailing '\0' */
    size_t log_name_max_len = MAX_LOG_NAME_LEN - 1;

    /* Create a logger with a name longer than the maximum */
    auto logger = slog::logger("logger_name_with_more_than_the maximum_length");

    /* Create a string with the current logger name*/
    std::string name(logger.get_name());

    /* Check that the namestores was truncated to the maximum */
    EXPECT_EQ(name.size(), log_name_max_len);
    EXPECT_EQ(name, "logger_name_with_mor");


    /* Create a logger with a name shorter than the maximum */
    auto logger2 = slog::logger("test_name");

    /* Create a string with the current logger name*/
    name = std::string(logger2.get_name());

    /* Check that the name stores the full name */
    EXPECT_EQ(name.size(), 9);
    EXPECT_EQ(name, "test_name");
}


TEST(SmallLogTest, logger_level) {

    /* Create a logger */
    auto logger = slog::logger("test_logger");
    /* Check that the default level is INFO */
    EXPECT_EQ(logger.get_Level(), slog::logger::level::info);

    /* Set the level to DEBUG */
    logger.set_Level(slog::logger::level::debug);
    /* Check that the level is now DEBUG */
    EXPECT_EQ(logger.get_Level(), slog::logger::level::debug);

    /* Set the level to TRACE */
    logger.set_Level(slog::logger::level::trace);
    /* Check that the level is now TRACE */
    EXPECT_EQ(logger.get_Level(), slog::logger::level::trace);

    /* Set the level to WARN */
    logger.set_Level(slog::logger::level::warn);
    /* Check that the level is now WARN */
    EXPECT_EQ(logger.get_Level(), slog::logger::level::warn);

    /* Set the level to ERROR */
    logger.set_Level(slog::logger::level::error);
    /* Check that the level is now ERROR */
    EXPECT_EQ(logger.get_Level(), slog::logger::level::error);

    /* Set the level to FATAL */
    logger.set_Level(slog::logger::level::fatal);
    /* Check that the level is now FATAL */
    EXPECT_EQ(logger.get_Level(), slog::logger::level::fatal);

    /* Set the level to DISABLED */
    logger.set_Level(slog::logger::level::disabled);
    /* Check that the level is now DISABLED */
    EXPECT_EQ(logger.get_Level(), slog::logger::level::disabled);
}


TEST(SmallLogTest, logger_add_appender) {

    /* Create a logger */
    auto logger = slog::logger("test_logger");

    /* String stream to store the log output */
    std::stringstream ss;

    /* Create a lambda function to be used as an appender */
    auto appender = [&ss](const char *msg) {
        ss << msg;
    };

    /* Check that we can add as much appenders as defined */
    uint8_t i;

    for(i = 0; i < MAX_NBR_LOG_APPENDER; i++) {
        EXPECT_TRUE(logger.add_appender(appender));
    }

    /* At this point the appenders list must already be full */
    EXPECT_FALSE(logger.add_appender(appender));
}


TEST(SmallLogTest, logger_no_time_multiple_appenders) {
    /* By not setting a time provider the time won't be printed as a consequence 
     * We will also add multiple appenders and check all will receive the messages */


    /* Create a logger */
    auto logger = slog::logger("test_logger");

    /* String stream to store the log output */
    std::stringstream ss;
    std::stringstream ss2;

    /* Create a lambda function to be used as an appender */
    auto appender = [&ss](const char *msg) {
        ss << msg;
    };
    /* Create a lambda function to be used as an appender */
    auto appender2 = [&ss2](const char *msg) {
        ss2 << msg;
    };

    /* Add the appenders to the logger */
    logger.add_appender(appender);
    logger.add_appender(appender2);

    ss.str(std::string()); // clear the stringstream
    /* Log the message as char* */
    logger.log(slog::logger::level::info, "log message test");

    /* Check both appenders received the message 
     * and both with no time information as expected */
    EXPECT_EQ(ss.str(), "\n[INFO ][test_logger] log message test");
    EXPECT_EQ(ss2.str(), "\n[INFO ][test_logger] log message test");
}


TEST(SmallLogTest, logger_time_n_date) {
    /* Test the time and date are printed correctly */

    /* Create a logger */
    auto logger = slog::logger("test_logger");

    /* String stream to store the log output */
    std::stringstream ss;

    /* Create a lambda function to be used as an appender */
    auto appender = [&ss](const char *msg) {
        ss << msg;
    };

    /* Create a lambda function to be used as time provider */
    auto time_provider = []() {
        slog::timedate td;
        td.setMYear(2024);
        td.setMMonth(1);
        td.setMDay(30);
        td.setMHour(23);
        td.setMMinute(25);
        td.setMSecond(16);
        td.setMMillisecond(753);
        return td;
    };

    /* Add the appender to the logger */
    logger.add_appender(appender);

    /* If there is no time providers time and date will not be printed */
    ss.str(std::string()); // clear the stringstream
    logger.log(slog::logger::level::info, "log message test");
    EXPECT_EQ(ss.str(), "\n[INFO ][test_logger] log message test");

    /* Add timer provider to the logger */
    logger.set_time_provider(time_provider);

    /* By default date will NOT be printed */
    ss.str(std::string()); // clear the stringstream
    logger.log(slog::logger::level::info, "log message test");
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] log message test");

    /* Explicitly set the date to be printed together with the time information */
    logger.set_print_date(true);
    
    ss.str(std::string()); // clear the stringstream
    logger.log(slog::logger::level::info, "log message test");
    EXPECT_EQ(ss.str(), "\n[2024/01/30 23:25:16.753][INFO ][test_logger] log message test");
}


TEST(SmallLogTest, logger_message_types) {
    /* We will test the log methods with the mutiple types supported */

    /* Create a logger */
    auto logger = slog::logger("test_logger");

    /* String stream to store the log output */
    std::stringstream ss;

    /* Create a lambda function to be used as an appender */
    auto appender = [&ss](const char *msg) {
        ss << msg;
    };

    /* Create a lambda function to be used as time provider */
    auto time_provider = []() {
        slog::timedate td;
        td.setMYear(2024);
        td.setMMonth(1);
        td.setMDay(30);
        td.setMHour(23);
        td.setMMinute(25);
        td.setMSecond(16);
        td.setMMillisecond(753);
        return td;
    };

    /* Add the appender to the logger */
    logger.add_appender(appender);
    /* Add timer provider to the logger */
    logger.set_time_provider(time_provider);

    /* Log the message as char pointer */
    ss.str(std::string()); // clear the stringstream
    logger.log(slog::logger::level::info, "char pointer test");
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] char pointer test");

    /* Log the message as string */
    ss.str(std::string()); // clear the stringstream
    std::string str = "string test";
    logger.log(slog::logger::level::info, str);
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] string test");

    /* Log the message as string_view */
    ss.str(std::string()); // clear the stringstream
    std::string_view str_vw = "string_view test";
    logger.log(slog::logger::level::info, str_vw);
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] string_view test");

    /* << operator with char * */
    ss.str(std::string()); // clear the stringstream
    logger.log(slog::logger::level::info, "Operator <<") << " | char pointer test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] Operator << | char pointer test");

    /* << operator with string */
    ss.str(std::string()); // clear the stringstream
    std::string str2 = " | string2 test";
    logger.log(slog::logger::level::info, "Operator <<") << str2;
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] Operator << | string2 test");

    /* << operator with string_view */
    ss.str(std::string()); // clear the stringstream
    std::string_view str_vw2 = " | string_view2 test";
    logger.log(slog::logger::level::info, "Operator <<") << str_vw2;
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] Operator << | string_view2 test");

    /* << operator with std::chrono::seconds */
    ss.str(std::string()); // clear the stringstream
    std::chrono::seconds time_s = std::chrono::seconds(123);
    logger.log(slog::logger::level::info, "Operator << ") << time_s;
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] Operator << 123s");

    /* << operator with std::chrono::milliseconds */
    ss.str(std::string()); // clear the stringstream
    std::chrono::milliseconds time_ms = std::chrono::milliseconds(456);
    logger.log(slog::logger::level::info, "Operator << ") << time_ms;
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] Operator << 456ms");

    /* << operator with std::chrono::microseconds */
    ss.str(std::string()); // clear the stringstream
    std::chrono::microseconds time_us = std::chrono::microseconds(789);
    logger.log(slog::logger::level::info, "Operator << ") << time_us;
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] Operator << 789us");

    /* << operator with Radix binary */
    ss.str(std::string()); // clear the stringstream
    logger.log(slog::logger::level::info, "Operator << ") << slog::logger::radix::bin << 170;
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] Operator << 0b10101010");

    /* << operator with Radix octal */
    ss.str(std::string()); // clear the stringstream
    logger.log(slog::logger::level::info, "Operator << ") << slog::logger::radix::oct << 170;
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] Operator << 0o252");

    /* << operator with Radix decimal */
    ss.str(std::string()); // clear the stringstream
    logger.log(slog::logger::level::info, "Operator << ") << slog::logger::radix::dec << 0xAA;
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] Operator << 170");

    /* << operator with Radix hexadecimal */
    ss.str(std::string()); // clear the stringstream
    logger.log(slog::logger::level::info, "Operator << ") << slog::logger::radix::hex << 170;
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] Operator << 0xAA");
}


TEST(SmallLogTest, logger_trace) {
    /* Check the logs with TRACE level will only be printe when current level is:
     * - TRACE
     * and ignored in all the other levels*/

    /* Create a logger */
    auto logger = slog::logger("test_logger");

    /* String stream to store the log output */
    std::stringstream ss;

    /* Create a lambda function to be used as an appender */
    auto appender = [&ss](const char *msg) {
        ss << msg;
    };

    /* Create a lambda function to be used as time provider */
    auto time_provider = []() {
        slog::timedate td;
        td.setMYear(2024);
        td.setMMonth(1);
        td.setMDay(30);
        td.setMHour(23);
        td.setMMinute(25);
        td.setMSecond(16);
        td.setMMillisecond(753);
        return td;
    };

    /* Add the appender to the logger */
    logger.add_appender(appender);
    /* Add timer provider to the logger */
    logger.set_time_provider(time_provider);

    /* TRACE message in TRACE current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::trace);
    logger.log(slog::logger::level::trace, "Trace message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][TRACE][test_logger] Trace message test");

    /* TRACE message in DEBUG current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::debug);
    logger.log(slog::logger::level::trace, "Trace message ") << "test";
    EXPECT_EQ(ss.str(), "");

    /* TRACE message in INFO current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::info);
    logger.log(slog::logger::level::trace, "Trace message ") << "test";
    EXPECT_EQ(ss.str(), "");

    /* TRACE message in WARN current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::warn);
    logger.log(slog::logger::level::trace, "Trace message ") << "test";
    EXPECT_EQ(ss.str(), "");

    /* TRACE message in ERROR current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::error);
    logger.log(slog::logger::level::trace, "Trace message ") << "test";
    EXPECT_EQ(ss.str(), "");

    /* TRACE message in FATAL current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::fatal);
    logger.log(slog::logger::level::trace, "Trace message ") << "test";
    EXPECT_EQ(ss.str(), "");

    /* TRACE message in DISABLED current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::disabled);
    logger.log(slog::logger::level::trace, "Trace message ") << "test";
    EXPECT_EQ(ss.str(), "");
}


TEST(SmallLogTest, logger_debug) {
    /* Check the logs with DEBUG level will only be printe when current level is:
     * - TRACE
     * - DEBUG
     * and ignored in all the other levels*/

    /* Create a logger */
    auto logger = slog::logger("test_logger");

    /* String stream to store the log output */
    std::stringstream ss;

    /* Create a lambda function to be used as an appender */
    auto appender = [&ss](const char *msg) {
        ss << msg;
    };

    /* Create a lambda function to be used as time provider */
    auto time_provider = []() {
        slog::timedate td;
        td.setMYear(2024);
        td.setMMonth(1);
        td.setMDay(30);
        td.setMHour(23);
        td.setMMinute(25);
        td.setMSecond(16);
        td.setMMillisecond(753);
        return td;
    };

    /* Add the appender to the logger */
    logger.add_appender(appender);
    /* Add timer provider to the logger */
    logger.set_time_provider(time_provider);

    /* DEBUG message in TRACE current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::trace);
    logger.log(slog::logger::level::debug, "Debug message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][DEBUG][test_logger] Debug message test");

    /* DEBUG message in DEBUG current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::debug);
    logger.log(slog::logger::level::debug, "Debug message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][DEBUG][test_logger] Debug message test");

    /* DEBUG message in INFO current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::info);
    logger.log(slog::logger::level::debug, "Debug message ") << "test";
    EXPECT_EQ(ss.str(), "");

    /* DEBUG message in WARN current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::warn);
    logger.log(slog::logger::level::debug, "Debug message ") << "test";
    EXPECT_EQ(ss.str(), "");

    /* DEBUG message in ERROR current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::error);
    logger.log(slog::logger::level::debug, "Debug message ") << "test";
    EXPECT_EQ(ss.str(), "");

    /* DEBUG message in FATAL current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::fatal);
    logger.log(slog::logger::level::debug, "Debug message ") << "test";
    EXPECT_EQ(ss.str(), "");

    /* DEBUG message in DISABLED current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::disabled);
    logger.log(slog::logger::level::debug, "Debug message ") << "test";
    EXPECT_EQ(ss.str(), "");
}


TEST(SmallLogTest, logger_info) {
    /* Check the logs with INFO level will only be printe when current level is:
     * - TRACE
     * - DEBUG
     * - INFO
     * and ignored in all the other levels*/

    /* Create a logger */
    auto logger = slog::logger("test_logger");

    /* String stream to store the log output */
    std::stringstream ss;

    /* Create a lambda function to be used as an appender */
    auto appender = [&ss](const char *msg) {
        ss << msg;
    };

    /* Create a lambda function to be used as time provider */
    auto time_provider = []() {
        slog::timedate td;
        td.setMYear(2024);
        td.setMMonth(1);
        td.setMDay(30);
        td.setMHour(23);
        td.setMMinute(25);
        td.setMSecond(16);
        td.setMMillisecond(753);
        return td;
    };

    /* Add the appender to the logger */
    logger.add_appender(appender);
    /* Add timer provider to the logger */
    logger.set_time_provider(time_provider);

    /* INFO message in TRACE current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::trace);
    logger.log(slog::logger::level::info, "Info message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] Info message test");

    /* INFO message in DEBUG current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::debug);
    logger.log(slog::logger::level::info, "Info message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] Info message test");

    /* INFO message in INFO current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::info);
    logger.log(slog::logger::level::info, "Info message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] Info message test");

    /* INFO message in WARN current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::warn);
    logger.log(slog::logger::level::info, "Info message ") << "test";
    EXPECT_EQ(ss.str(), "");

    /* INFO message in ERROR current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::error);
    logger.log(slog::logger::level::info, "Info message ") << "test";
    EXPECT_EQ(ss.str(), "");

    /* INFO message in FATAL current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::fatal);
    logger.log(slog::logger::level::info, "Info message ") << "test";
    EXPECT_EQ(ss.str(), "");

    /* INFO message in DISABLED current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::disabled);
    logger.log(slog::logger::level::info, "Info message ") << "test";
    EXPECT_EQ(ss.str(), "");
}


TEST(SmallLogTest, logger_warn) {
    /* Check the logs with WARN level will only be printe when current level is:
     * - TRACE
     * - DEBUG
     * - INFO
     * - WARN
     * and ignored in all the other levels*/

    /* Create a logger */
    auto logger = slog::logger("test_logger");

    /* String stream to store the log output */
    std::stringstream ss;

    /* Create a lambda function to be used as an appender */
    auto appender = [&ss](const char *msg) {
        ss << msg;
    };

    /* Create a lambda function to be used as time provider */
    auto time_provider = []() {
        slog::timedate td;
        td.setMYear(2024);
        td.setMMonth(1);
        td.setMDay(30);
        td.setMHour(23);
        td.setMMinute(25);
        td.setMSecond(16);
        td.setMMillisecond(753);
        return td;
    };

    /* Add the appender to the logger */
    logger.add_appender(appender);
    /* Add timer provider to the logger */
    logger.set_time_provider(time_provider);

    /* WARN message in TRACE current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::trace);
    logger.log(slog::logger::level::warn, "Warn message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][WARN ][test_logger] Warn message test");

    /* WARN message in DEBUG current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::debug);
    logger.log(slog::logger::level::warn, "Warn message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][WARN ][test_logger] Warn message test");

    /* WARN message in INFO current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::info);
    logger.log(slog::logger::level::warn, "Warn message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][WARN ][test_logger] Warn message test");

    /* WARN message in WARN current level */
    ss.str(std::string());
    logger.set_Level(slog::logger::level::warn);
    logger.log(slog::logger::level::warn, "Warn message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][WARN ][test_logger] Warn message test");

    /* WARN message in ERROR current level */
    ss.str(std::string());
    logger.set_Level(slog::logger::level::error);
    logger.log(slog::logger::level::warn, "Warn message ") << "test";
    EXPECT_EQ(ss.str(), "");

    /* WARN message in FATAL current level */
    ss.str(std::string());
    logger.set_Level(slog::logger::level::fatal);
    logger.log(slog::logger::level::warn, "Warn message ") << "test";
    EXPECT_EQ(ss.str(), "");

    /* WARN message in DISABLED current level */
    ss.str(std::string());
    logger.set_Level(slog::logger::level::disabled);
    logger.log(slog::logger::level::warn, "Warn message ") << "test";
    EXPECT_EQ(ss.str(), "");
}


TEST(SmallLogTest, logger_error) {
    /* Check the logs with ERROR level will only be printe when current level is:
     * - TRACE
     * - DEBUG
     * - INFO
     * - WARN
     * - ERROR
     * and ignored in all the other levels*/

    /* Create a logger */
    auto logger = slog::logger("test_logger");

    /* String stream to store the log output */
    std::stringstream ss;

    /* Create a lambda function to be used as an appender */
    auto appender = [&ss](const char *msg) {
        ss << msg;
    };

    /* Create a lambda function to be used as time provider */
    auto time_provider = []() {
        slog::timedate td;
        td.setMYear(2024);
        td.setMMonth(1);
        td.setMDay(30);
        td.setMHour(23);
        td.setMMinute(25);
        td.setMSecond(16);
        td.setMMillisecond(753);
        return td;
    };

    /* Add the appender to the logger */
    logger.add_appender(appender);
    /* Add timer provider to the logger */
    logger.set_time_provider(time_provider);

    /* ERROR message in TRACE current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::trace);
    logger.log(slog::logger::level::error, "Error message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][ERROR][test_logger] Error message test");

    /* ERROR message in DEBUG current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::debug);
    logger.log(slog::logger::level::error, "Error message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][ERROR][test_logger] Error message test");

    /* ERROR message in INFO current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::info);
    logger.log(slog::logger::level::error, "Error message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][ERROR][test_logger] Error message test");

    /* ERROR message in WARN current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::warn);
    logger.log(slog::logger::level::error, "Error message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][ERROR][test_logger] Error message test");

    /* ERROR message in ERROR current level */
    ss.str(std::string());
    logger.set_Level(slog::logger::level::error);
    logger.log(slog::logger::level::error, "Error message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][ERROR][test_logger] Error message test");

    /* ERROR message in FATAL current level */
    ss.str(std::string());
    logger.set_Level(slog::logger::level::fatal);
    logger.log(slog::logger::level::error, "Error message ") << "test";
    EXPECT_EQ(ss.str(), "");

    /* ERROR message in DISABLED current level */
    ss.str(std::string());
    logger.set_Level(slog::logger::level::disabled);
    logger.log(slog::logger::level::error, "Error message ") << "test";
    EXPECT_EQ(ss.str(), "");
}


TEST(SmallLogTest, logger_fatal) {
    /* Check the logs with FATAL level will only be printe when current level is:
     * - TRACE
     * - DEBUG
     * - INFO
     * - WARN
     * - ERROR
     * - FATAL
     * and inly ignored in DISABLED*/

    /* Create a logger */
    auto logger = slog::logger("test_logger");

    /* String stream to store the log output */
    std::stringstream ss;

    /* Create a lambda function to be used as an appender */
    auto appender = [&ss](const char *msg) {
        ss << msg;
    };

    /* Create a lambda function to be used as time provider */
    auto time_provider = []() {
        slog::timedate td;
        td.setMYear(2024);
        td.setMMonth(1);
        td.setMDay(30);
        td.setMHour(23);
        td.setMMinute(25);
        td.setMSecond(16);
        td.setMMillisecond(753);
        return td;
    };

    /* Add the appender to the logger */
    logger.add_appender(appender);
    /* Add timer provider to the logger */
    logger.set_time_provider(time_provider);

    /* FATAL message in TRACE current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::trace);
    logger.log(slog::logger::level::fatal, "Fatal message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][FATAL][test_logger] Fatal message test");

    /* FATAL message in DEBUG current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::debug);
    logger.log(slog::logger::level::fatal, "Fatal message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][FATAL][test_logger] Fatal message test");

    /* FATAL message in INFO current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::info);
    logger.log(slog::logger::level::fatal, "Fatal message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][FATAL][test_logger] Fatal message test");

    /* FATAL message in WARN current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::warn);
    logger.log(slog::logger::level::fatal, "Fatal message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][FATAL][test_logger] Fatal message test");

    /* FATAL message in ERROR current level */
    ss.str(std::string());
    logger.set_Level(slog::logger::level::error);
    logger.log(slog::logger::level::fatal, "Fatal message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][FATAL][test_logger] Fatal message test");

    /* FATAL message in FATAL current level */
    ss.str(std::string());
    logger.set_Level(slog::logger::level::fatal);
    logger.log(slog::logger::level::fatal, "Fatal message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][FATAL][test_logger] Fatal message test");

    /* FATAL message in DISABLED current level */
    ss.str(std::string());
    logger.set_Level(slog::logger::level::disabled);
    logger.log(slog::logger::level::fatal, "Fatal message ") << "test";
    EXPECT_EQ(ss.str(), "");
}


TEST(SmallLogTest, logger_disabled) {
    /* Check the logs with DISABLED (wich doesn't make sense) level will allways be ignored */

    /* Create a logger */
    auto logger = slog::logger("test_logger");

    /* String stream to store the log output */
    std::stringstream ss;

    /* Create a lambda function to be used as an appender */
    auto appender = [&ss](const char *msg) {
        ss << msg;
    };

    /* Create a lambda function to be used as time provider */
    auto time_provider = []() {
        slog::timedate td;
        td.setMYear(2024);
        td.setMMonth(1);
        td.setMDay(30);
        td.setMHour(23);
        td.setMMinute(25);
        td.setMSecond(16);
        td.setMMillisecond(753);
        return td;
    };

    /* Add the appender to the logger */
    logger.add_appender(appender);
    /* Add timer provider to the logger */
    logger.set_time_provider(time_provider);

    /* DISABLED message in TRACE current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::trace);
    logger.log(slog::logger::level::disabled, "Disabled message ") << "test";
    EXPECT_EQ(ss.str(), "");

    /* DISABLED message in DISABLED current level */
    ss.str(std::string()); // clear the stringstream
    logger.set_Level(slog::logger::level::disabled);
    logger.log(slog::logger::level::disabled, "Disabled message ") << "test";
    EXPECT_EQ(ss.str(), "");
}

TEST(SmallLogTest, logger_makros) {
    /* Check logging with the makros will work the same way */

    /* Create a logger */
    auto logger = slog::logger("test_logger");

    /* String stream to store the log output */
    std::stringstream ss;

    /* Create a lambda function to be used as an appender */
    auto appender = [&ss](const char *msg) {
        ss << msg;
    };

    /* Create a lambda function to be used as time provider */
    auto time_provider = []() {
        slog::timedate td;
        td.setMYear(2024);
        td.setMMonth(1);
        td.setMDay(30);
        td.setMHour(23);
        td.setMMinute(25);
        td.setMSecond(16);
        td.setMMillisecond(753);
        return td;
    };

    /* Add the appender to the logger */
    logger.add_appender(appender);
    /* Add timer provider to the logger */
    logger.set_time_provider(time_provider);
    /* Set level as TRACE */
    logger.set_Level(slog::logger::level::trace);

    /* TRACE message */
    ss.str(std::string()); // clear the stringstream    
    SLOG_TRACE(logger, "Trace message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][TRACE][test_logger] Trace message test");

    /* DEBUG message */
    ss.str(std::string()); // clear the stringstream
    SLOG_DEBUG(logger, "Debug message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][DEBUG][test_logger] Debug message test");

    /* INFO message */
    ss.str(std::string()); // clear the stringstream
    SLOG_INFO(logger, "Info message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][INFO ][test_logger] Info message test");

    /* WARN message */
    ss.str(std::string()); // clear the stringstream
    SLOG_WARN(logger, "Warn message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][WARN ][test_logger] Warn message test");

    /* ERROR message */
    ss.str(std::string());
    SLOG_ERROR(logger, "Error message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][ERROR][test_logger] Error message test");

    /* FATAL message */
    ss.str(std::string());
    SLOG_FATAL(logger, "Fatal message ") << "test";
    EXPECT_EQ(ss.str(), "\n[23:25:16.753][FATAL][test_logger] Fatal message test");
}

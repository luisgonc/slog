#include <iostream>
#include "slog.h"

int main() {
    std::cout << "Hello, Slog!" << std::endl;

    auto logger = slog::logger("Main123456Main123456Main");

    std::cout << "Logger name: " << logger.get_name() << std::endl;

    std::cout << "Logger level: " << static_cast<int>(logger.get_Level()) << std::endl;

    auto callback = [](const char* msg) {
        std::cout << msg ;
    };

    logger.add_appender(callback);

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

    logger.set_time_provider(time_provider);

    logger.log(slog::logger::level::INFO, "Hello, Slog!") << " | This is a test message";
    logger.log(slog::logger::level::ERROR, "Pimbas erros!!####!");

    std::string cuca= "ipsum lerem darion";
    logger.log(slog::logger::level::ERROR, cuca);

    std::string_view cuca_view = "e plurribus unum";
    logger.log(slog::logger::level::ERROR, cuca_view);

    logger.log(slog::logger::level::ERROR, "Vamos la entao -> ") << cuca;

    logger.log(slog::logger::level::ERROR, "Vamos la entao_view -> ") << cuca_view << " " << 21358;

    return 0;
}

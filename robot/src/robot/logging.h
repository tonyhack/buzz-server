#ifndef BUZZ_ROBOT_ROBOT_LOGGING_H
#define BUZZ_ROBOT_ROBOT_LOGGING_H

#include <string>
#include <brickred/log_core.h>

namespace robot {

class Logging {
public:
    struct LogLevel {
        enum type {
            MIN = 0,

            TRACE = 0,
            DEBUG,
            INFO,
            WARNING,
            ERROR,

            MAX
        };
    };

    struct Logger {
        enum type {
            MIN = 0,

            MAIN = 0,

            MAX
        };
    };

    static size_t logFormatter(char *buffer, size_t buffer_size, int level,
        const char *filename, int line, const char *function,
        const char *format, va_list args);

    static bool initMainLogger(const std::string &log_file_path,
                               int log_level, bool log_stderr);
};

}

#define LOG_TRACE(_format, ...) \
    brickred::LogCore::getInstance()->log(robot::Logging::Logger::MAIN, \
        robot::Logging::LogLevel::TRACE, __FILE__, __LINE__, __func__, \
        _format, ##__VA_ARGS__)
#define LOG_DEBUG(_format, ...) \
    brickred::LogCore::getInstance()->log(robot::Logging::Logger::MAIN, \
        robot::Logging::LogLevel::DEBUG, __FILE__, __LINE__, __func__, \
        _format, ##__VA_ARGS__)
#define LOG_INFO(_format, ...) \
    brickred::LogCore::getInstance()->log(robot::Logging::Logger::MAIN, \
        robot::Logging::LogLevel::INFO, __FILE__, __LINE__, __func__, \
        _format, ##__VA_ARGS__)
#define LOG_WARNING(_format, ...) \
    brickred::LogCore::getInstance()->log(robot::Logging::Logger::MAIN, \
        robot::Logging::LogLevel::WARNING, __FILE__, __LINE__, __func__, \
        _format, ##__VA_ARGS__)
#define LOG_ERROR(_format, ...) \
    brickred::LogCore::getInstance()->log(robot::Logging::Logger::MAIN, \
        robot::Logging::LogLevel::ERROR, __FILE__, __LINE__, __func__, \
        _format, ##__VA_ARGS__)

#endif


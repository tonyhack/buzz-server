#ifndef _BRICKRED_BUILD_DISABLE_BASE_LOG_

#include <brickred/base_logger.h>

#include <cstdio>

namespace brickred {

BRICKRED_SINGLETON2_IMPL(BaseLogger)

static void defaultLogFunc(int level, const char *format, va_list args) {
    if (level < BaseLogger::LogLevel::MIN ||
        level >= BaseLogger::LogLevel::MAX) {
        return;
    }

    static const char *log_level_string[] = {
        "DEBUG",
        "INFO",
        "WARNING",
        "ERROR",
    };

    ::fprintf(stderr, "[%s] ", log_level_string[level]);
    ::vfprintf(stderr, format, args);
    ::fprintf(stderr, "\n");
}

BaseLogger::BaseLogger() : log_func_(defaultLogFunc)
{
}

BaseLogger::~BaseLogger()
{
}

void BaseLogger::setLogFunc(LogFunc log_func)
{
    log_func_ = log_func;
}

void BaseLogger::log(int level, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_func_(level, format, args);
    va_end(args);
}

} // namespace brickred

#endif // _BRICKRED_BUILD_DISABLE_BASE_LOG_

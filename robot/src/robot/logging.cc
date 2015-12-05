#include "robot/logging.h"

#include <cstdio>
#include <cstring>
#include <brickred/log_core.h>
#include <brickred/log_file_sink.h>
#include <brickred/log_stderr_sink.h>
#include <brickred/timestamp.h>
#include <brickred/unique_ptr.h>

namespace robot {

using brickred::LogCore;
using brickred::LogFileSink;
using brickred::LogStderrSink;
using brickred::Timestamp;
using brickred::UniquePtr;

size_t Logging::logFormatter(char *buffer, size_t buffer_size, int level,
    const char *filename, int line, const char *function,
    const char *format, va_list args)
{
    if (level < LogLevel::MIN || level >= LogLevel::MAX) {
        return 0;
    }

    static const char *log_level_string[] = {
        "T", "D", "I", "W", "E"
    };

    Timestamp now;
    now.setNow();

    char time_buffer[1024];
    Timestamp::format(time_buffer, sizeof(time_buffer),
        "%H:%M:%S", now.getSecond());

    size_t count = 0;
    count += ::snprintf(buffer, buffer_size, "[%s][%s:%03ld][%s:%d](%s) ",
        log_level_string[level], time_buffer, now.getMilliSecond(),
        ::strrchr(filename, '/') + 1, line, function);
    if (count + 1 >= buffer_size) {
        return count;
    }

    count += ::vsnprintf(buffer + count, buffer_size - count, format, args);
    if (count + 1 >= buffer_size) {
        return count;
    }

    count += ::snprintf(buffer + count, buffer_size - count, "\n");

    return count;
}

bool Logging::initMainLogger(const std::string &log_file_path,
                             int log_level, bool log_stderr)
{
    if (LogCore::getInstance()->registerLogger(Logger::MAIN,
            logFormatter) == false) {
        return false;
    }

    // file log
    UniquePtr<LogFileSink> file_sink(new LogFileSink(log_file_path));
    if (file_sink->openFile() == false) {
        return false;
    }
    if (LogCore::getInstance()->addSink(Logger::MAIN,
            file_sink.get(), log_level) == false) {
        return false;
    }
    file_sink.release();

    // stderr log
    if (log_stderr) {
        UniquePtr<LogStderrSink> stderr_sink(new LogStderrSink());
        if (LogCore::getInstance()->addSink(Logger::MAIN,
                stderr_sink.get(), log_level) == false) {
            return false;
        }
        stderr_sink.release();
    }

    return true;
}

} // namespace robot


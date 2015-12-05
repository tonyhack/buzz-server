#ifndef BRICKRED_LOG_CORE_H
#define BRICKRED_LOG_CORE_H

#include <cstddef>
#include <cstdarg>

#include <brickred/class_util.h>
#include <brickred/unique_ptr.h>

namespace brickred {

class LogSink;

class LogCore {
public:
    typedef size_t (*LogFormatter)(
        char *buffer, size_t buffer_size, int level,
        const char *filename, int line, const char *function,
        const char *format, va_list args
    );

    bool registerLogger(int logger_id, LogFormatter formatter = NULL);
    void removeLogger(int logger_id);
    bool addSink(int logger_id, LogSink *sink, int level_filter);
    void log(int logger_id, int level, const char *filename,
             int line, const char *function, const char *format, ...);

private:
    BRICKRED_SINGLETON2(LogCore)
    
    class Impl;
    UniquePtr<Impl> pimpl_;
};

} // namespace brickred

#endif

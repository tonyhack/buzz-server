#ifndef BRICKRED_LOG_SINK_H
#define BRICKRED_LOG_SINK_H

#include <cstddef>

#include <brickred/class_util.h>

namespace brickred {

class LogSink {
public:
    LogSink() {}
    virtual ~LogSink() {}

    virtual void log(const char *buffer, size_t size) = 0;

private:
    BRICKRED_NONCOPYABLE(LogSink)
};

} // namespace brickred

#endif

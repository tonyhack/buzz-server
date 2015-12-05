#ifndef BRICKRED_LOG_STDERR_SINK_H
#define BRICKRED_LOG_STDERR_SINK_H

#include <cstddef>

#include <brickred/log_sink.h>
#include <brickred/unique_ptr.h>

namespace brickred {

class LogStderrSink : public LogSink {
public:
    LogStderrSink();
    virtual ~LogStderrSink();

    virtual void log(const char *buffer, size_t size);
};

} // namespace brickred

#endif

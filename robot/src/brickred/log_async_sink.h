#ifndef BRICKRED_LOG_ASYNC_SINK_H
#define BRICKRED_LOG_ASYNC_SINK_H

#include <cstddef>

#include <brickred/log_sink.h>
#include <brickred/unique_ptr.h>

namespace brickred {

class LogAsyncSink : public LogSink {
public:
    LogAsyncSink(LogSink *adapted_sink, size_t queue_size = 100);
    virtual ~LogAsyncSink();

    virtual void log(const char *buffer, size_t size);

private:
    class Impl;
    UniquePtr<Impl> pimpl_;
};

} // namespace brickred

#endif

#include <brickred/log_stderr_sink.h>

#include <cstdio>

namespace brickred {

LogStderrSink::LogStderrSink()
{
}

LogStderrSink::~LogStderrSink()
{
}

void LogStderrSink::log(const char *buffer, size_t size)
{
    ::fwrite(buffer, size, 1, stderr);
}

} // namespace brickred

#ifndef BRICKRED_TIMESTAMP_H
#define BRICKRED_TIMESTAMP_H

#include <stdint.h>
#include <cstddef>
#include <ctime>
#include <string>

namespace brickred {

class Timestamp {
public:
    Timestamp();
    ~Timestamp();

    void setNow();

    time_t getSecond() const { return second_; }
    int64_t getMilliSecond() const { return nanosecond_ / 1000000; }

    Timestamp &operator+=(int64_t millisecond);
    bool operator<(const Timestamp &other) const;
    Timestamp operator+(int64_t millisecond) const;

    int64_t distanceSecond(const Timestamp &other) const;
    int64_t distanceMillisecond(const Timestamp &other) const;

    static size_t format(char *buffer, size_t size,
                         const char *format, time_t second);

private:
    time_t second_;
    int64_t nanosecond_;
};

} // namespace brickred

#endif

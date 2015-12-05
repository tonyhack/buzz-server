#ifndef BRICKRED_IO_SERVICE_H
#define BRICKRED_IO_SERVICE_H

#include <stdint.h>

#include <brickred/class_util.h>
#include <brickred/function.h>
#include <brickred/unique_ptr.h>

namespace brickred {

class IODevice;

class IOService {
public:
    typedef int64_t TimerId;
    typedef Function<void (TimerId)> TimerCallback;

    IOService();
    ~IOService();

    void loop();
    void quit();

    TimerId startTimer(int timeout_ms, const TimerCallback &timer_cb,
                       int call_times = -1);
    void stopTimer(TimerId timer_id);

private:
    friend class IODevice;
    bool addIODevice(IODevice *io_device);
    bool removeIODevice(IODevice *io_device);
    bool updateIODevice(IODevice *io_device);

private:
    BRICKRED_NONCOPYABLE(IOService)

    class Impl;
    UniquePtr<Impl> pimpl_;
};

} // namespace brickred

#endif

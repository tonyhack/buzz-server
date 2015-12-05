#ifndef BUZZ_ROBOT_ROBOT_CLIENT_APP_H 
#define BUZZ_ROBOT_ROBOT_CLIENT_APP_H

#include <stdint.h>
#include <cstddef>
#include <brickred/class_util.h>
#include <brickred/unique_ptr.h>
#include <brickred/function.h>

namespace brickred {

class SocketAddress;

} // namespace brickred

namespace robot {

class ClientApp {
public:
    struct Protocol {
        enum type {
            MIN = 0,

            HTTP = 0,
            BUZZ,

            MAX
        };
    };

    typedef brickred::Function<void (int64_t)> TimerCallback;

    bool init();
    void run();

    int64_t connect(int64_t actor_no, const brickred::SocketAddress &addr,
                    bool *complete, Protocol::type protocol);
    void sendMessage(int64_t socket_id, const char *message, size_t size);
    void disconnect(int64_t socket_id);

    int64_t startTimer(int timeout_ms, TimerCallback timer_cb,
                       int call_times = -1);
    void stopTimer(int64_t timer_id);

    uint32_t rand(uint32_t max);
    uint32_t uniformRand(uint32_t min, uint32_t max);

private:
    BRICKRED_SINGLETON2(ClientApp)

    class Impl;
    brickred::UniquePtr<Impl> pimpl_;
};

} // namespace robot

#endif


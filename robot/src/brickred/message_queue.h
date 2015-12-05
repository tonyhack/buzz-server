#ifndef BRICKRED_MESSAGE_QUEUE_H
#define BRICKRED_MESSAGE_QUEUE_H

#include <cstddef>

#include <brickred/concurrent_queue.h>
#include <brickred/exception.h>
#include <brickred/function.h>
#include <brickred/io_service.h>
#include <brickred/self_pipe.h>

namespace brickred {

template <class T>
class MessageQueue {
public:
    typedef Function<void (MessageQueue *)> RecvMessageCallback;

    explicit MessageQueue(IOService &io_service, size_t max_size = 0) :
        io_service_(&io_service), queue_(max_size)
    {
        if (pipe_.open() == false ||
            pipe_.setNonblock() == false ||
            pipe_.setCloseOnExec() == false) {
            throw SystemErrorException(
                "create message queue failed in self pipe init"
            );
        }

        pipe_.setReadCallback(BRICKRED_BIND_TEMPLATE_MEM_FUNC(
            &MessageQueue::pipeReadCallback, this));
        pipe_.attachIOService(*io_service_);
    }

    ~MessageQueue()
    {
    }

    IOService *getIOService() const
    {
        return io_service_;
    }

    void setRecvMessageCallback(const RecvMessageCallback &recv_message_cb)
    {
        recv_message_cb_ = recv_message_cb;
    }

    // may block
    void push(const T &item)
    {
        queue_.push(item);
        pipe_.write("1", 1);
    }

    // never block
    bool pop(T &item)
    {
        return queue_.popIfNotEmpty(item);
    }

private:
    void pipeReadCallback(IODevice *io_device)
    {
        char buffer[1024];
        while (pipe_.read(buffer, sizeof(buffer)) > 0) ;
        if (recv_message_cb_) {
            recv_message_cb_(this);
        }
    }

private:
    IOService *io_service_;
    ConcurrentQueue<T> queue_;
    SelfPipe pipe_;
    RecvMessageCallback recv_message_cb_;
};

} // namespace brickred

#endif

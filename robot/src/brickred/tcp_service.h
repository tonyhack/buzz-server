#ifndef BRICKRED_TCP_SERVICE_H
#define BRICKRED_TCP_SERVICE_H

#include <stdint.h>
#include <cstddef>

#include <brickred/class_util.h>
#include <brickred/function.h>
#include <brickred/unique_ptr.h>

namespace brickred {

class DynamicBuffer;
class SocketAddress;
class IOService;

class TcpService {
public:
    class Context {
    public:
        Context() {}
        virtual ~Context() = 0;
    private:
        BRICKRED_NONCOPYABLE(Context)
    };

    typedef int64_t SocketId;
    typedef Function<void (TcpService *, SocketId, SocketId)>
            NewConnectionCallback;
    typedef Function<void (TcpService *, SocketId, DynamicBuffer *)>
            RecvMessageCallback;
    typedef Function<void (TcpService *, SocketId)>
            PeerCloseCallback;
    typedef Function<void (TcpService *, SocketId, int)>
            ErrorCallback;
    typedef Function<void (TcpService *, SocketId)>
            SendCompleteCallback;

    explicit TcpService(IOService &io_service);
    ~TcpService();

    IOService *getIOService() const;

    SocketId listen(const SocketAddress &addr);
    SocketId connect(const SocketAddress &addr);
    SocketId asyncConnect(const SocketAddress &addr, bool *complete,
                          int timeout_ms = -1);

    bool getLocalAddress(SocketId socket_id, SocketAddress *addr) const;
    bool getPeerAddress(SocketId socket_id, SocketAddress *addr) const;

    bool sendMessage(SocketId socket_id, const char *buffer, size_t size,
                     SendCompleteCallback send_complete_cb = NullFunction());
    bool sendMessageThenClose(SocketId socket_id,
                              const char *buffer, size_t size);
    void closeSocket(SocketId socket_id);

    Context *getContext(SocketId socket_id) const;
    bool setContext(SocketId socket_id, Context *context);

    void setNewConnectionCallback(const NewConnectionCallback &new_conn_cb);
    void setRecvMessageCallback(const RecvMessageCallback &recv_message_cb);
    void setPeerCloseCallback(const PeerCloseCallback &peer_close_cb);
    void setErrorCallback(const ErrorCallback &error_cb);

    void setRecvBufferInitSize(size_t size = 1024);
    void setRecvBufferExpandSize(size_t size = 1024);
    void setRecvBufferMaxSize(size_t size = 0);
    void setSendBufferInitSize(size_t size = 1024);
    void setSendBufferExpandSize(size_t size = 1024);

private:
    BRICKRED_NONCOPYABLE(TcpService)

    class Impl;
    UniquePtr<Impl> pimpl_;
};

} // namespace brickred

#endif

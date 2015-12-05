#ifndef BRICKRED_PROTOCOL_WEB_SOCKET_PROTOCOL_H
#define BRICKRED_PROTOCOL_WEB_SOCKET_PROTOCOL_H

#include <cstddef>
#include <map>
#include <string>

#include <brickred/class_util.h>
#include <brickred/string_util.h>
#include <brickred/tcp_service.h>
#include <brickred/unique_ptr.h>

namespace brickred {

class Random;

} // namespace brickred

namespace brickred {
namespace protocol {

class WebSocketProtocol {
public:
    struct Status {
        enum type {
            DETACHED,
            WAITING_HANDSHAKE_REQUEST,
            WAITING_HANDSHAKE_RESPONSE,
            CONNECTED,
            FINISHED,
            CLOSED,
            PENDING_ERROR,
            MAX
        };
    };

    struct RetCode {
        enum type {
            ERROR = -1,
            WAITING_MORE_DATA = 0,
            CONNECTION_ESTABLISHED,
            MESSAGE_READY,
            PEER_CLOSED,
            PING_FRAME,
            PONG_FRAME,
        };
    };

    typedef std::map<std::string, std::string,
                     string_util::CaseInsensitiveLess> HeaderMap;

    WebSocketProtocol();
    ~WebSocketProtocol();

    Status::type getStatus() const;
    void setHandshakeHeader(const std::string &key, const std::string &value);

    // send a handshake to the server
    bool startAsClient(TcpService &tcp_service,
                       TcpService::SocketId socket_id,
                       Random &random_generator,
                       const char *request_uri = "/");
    // wait a handshake from the client
    bool startAsServer(TcpService &tcp_service,
                       TcpService::SocketId socket_id);

    RetCode::type recvMessage(DynamicBuffer *buffer);
    bool retrieveMessage(DynamicBuffer *message);
    bool sendMessage(const char *buffer, size_t size);
    bool sendCloseFrame();
    bool sendPingFrame();

private:
    BRICKRED_NONCOPYABLE(WebSocketProtocol)

    class Impl;
    UniquePtr<Impl> pimpl_;
};

} // namespace protocol
} // namespace brickred

#endif

#include <brickred/protocol/web_socket_protocol.h>

#include <stdint.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>

#include <brickred/dynamic_buffer.h>
#include <brickred/random.h>
#include <brickred/socket_address.h>
#include <brickred/codec/base64.h>
#include <brickred/codec/sha1.h>
#include <brickred/protocol/http_message.h>
#include <brickred/protocol/http_protocol.h>
#include <brickred/protocol/http_request.h>
#include <brickred/protocol/http_response.h>

namespace brickred {
namespace protocol {

class WebSocketProtocol::Impl {
public:
    typedef WebSocketProtocol::Status Status;
    typedef WebSocketProtocol::RetCode RetCode;
    typedef WebSocketProtocol::HeaderMap HeaderMap;
    typedef int (WebSocketProtocol::Impl::*StatusHandler)(DynamicBuffer *);

    Impl();
    ~Impl();

    Status::type getStatus() const { return status_; }
    void setHandshakeHeader(const std::string &key, const std::string &value);

    bool startAsClient(TcpService &tcp_service,
                       TcpService::SocketId socket_id,
                       Random &random_generator,
                       const char *request_uri);
    bool startAsServer(TcpService &tcp_service,
                       TcpService::SocketId socket_id);

    RetCode::type recvMessage(DynamicBuffer *buffer);
    bool retrieveMessage(DynamicBuffer *message);
    bool sendMessage(const char *buffer, size_t size);
    bool sendCloseFrame();
    bool sendPingFrame();

public:
    bool sendPongFrame();
    int readHandshakeRequest(DynamicBuffer *buffer);
    int readHandshakeResponse(DynamicBuffer *buffer);
    int readFrame(DynamicBuffer *buffer);

private:
    static StatusHandler s_status_handler_[Status::MAX];

private:
    Status::type status_;
    TcpService *tcp_service_;
    TcpService::SocketId socket_id_;
    Random *random_generator_;
    HttpProtocol http_protocol_;
    HeaderMap handshake_headers_;
    DynamicBuffer message_;
    bool is_client_;
    bool close_frame_sent_;
    int last_op_code_;
    RetCode::type ret_code_;
};

///////////////////////////////////////////////////////////////////////////////
WebSocketProtocol::Impl::StatusHandler
WebSocketProtocol::Impl::s_status_handler_[] = {
    NULL,
    &WebSocketProtocol::Impl::readHandshakeRequest,
    &WebSocketProtocol::Impl::readHandshakeResponse,
    &WebSocketProtocol::Impl::readFrame,
    NULL,
    NULL,
    NULL
};

WebSocketProtocol::Impl::Impl() :
    status_(Status::DETACHED),
    tcp_service_(NULL), socket_id_(0), random_generator_(NULL),
    is_client_(false), close_frame_sent_(false), last_op_code_(-1),
    ret_code_(RetCode::ERROR)
{
}

WebSocketProtocol::Impl::~Impl()
{
}

void WebSocketProtocol::Impl::setHandshakeHeader(const std::string &key,
                                                 const std::string &value)
{
    handshake_headers_[key] = value;
}

bool WebSocketProtocol::Impl::startAsClient(
    TcpService &tcp_service, TcpService::SocketId socket_id,
    Random &random_generator, const char *request_uri)
{
    if (status_ != Status::DETACHED) {
        return false;
    }

    status_ = Status::WAITING_HANDSHAKE_RESPONSE;
    tcp_service_ = &tcp_service;
    socket_id_ = socket_id;
    random_generator_ = &random_generator;
    is_client_ = true;

    // send handshake
    HttpRequest request;
    request.setVersion(HttpMessage::Version::HTTP_1_1);
    request.setMethod(HttpRequest::Method::GET);
    request.setRequestUri(request_uri);
    request.setHeader("Upgrade", "websocket");
    request.setHeader("Connection", "Upgrade");
    request.setHeader("Sec-WebSocket-Version", "13");

    // set header 'Sec-WebSocket-Key' if not provided
    if (handshake_headers_.find("Sec-WebSocket-Key") ==
        handshake_headers_.end()) {
        std::vector<char> key(16);
        for (size_t i = 0; i < key.size(); ++i) {
            key[i] = random_generator_->nextInt(256);
        }
        setHandshakeHeader("Sec-WebSocket-Key",
                           codec::base64Encode(&key[0], key.size()));
    }
    // set header 'Host' if not provided
    if (handshake_headers_.find("Host") ==
        handshake_headers_.end()) {
        SocketAddress addr;
        if (tcp_service_->getPeerAddress(socket_id_, &addr) == false) {
            status_ = Status::PENDING_ERROR;
            return false;
        }
        setHandshakeHeader("Host",
            addr.getIp() + ":" + string_util::toString(addr.getPort()));
    }

    // set handshake headers
    for (HeaderMap::const_iterator iter = handshake_headers_.begin();
         iter != handshake_headers_.end(); ++iter) {
        request.setHeader(iter->first, iter->second);
    }

    DynamicBuffer buffer;
    HttpProtocol::writeMessage(request, &buffer);
    if (tcp_service_->sendMessage(socket_id_,
            buffer.readBegin(), buffer.readableBytes()) == false) {
        status_ = Status::PENDING_ERROR;
        return false;
    }

    return true;
}

bool WebSocketProtocol::Impl::startAsServer(
    TcpService &tcp_service, TcpService::SocketId socket_id)
{
    if (status_ != Status::DETACHED) {
        return false;
    }

    status_ = Status::WAITING_HANDSHAKE_REQUEST;
    tcp_service_ = &tcp_service;
    socket_id_ = socket_id;
    is_client_ = false;

    return true;
}

WebSocketProtocol::RetCode::type WebSocketProtocol::Impl::recvMessage(
    DynamicBuffer *buffer)
{
    for (;;) {
        StatusHandler func = s_status_handler_[status_];
        if (NULL == func) {
            return RetCode::ERROR;
        }

        int ret = (this->*func)(buffer);
        if (0 == ret) {
            // not finished, wait for more data
            return RetCode::WAITING_MORE_DATA;

        } else if (-1 == ret) {
            // error occured
            status_ = Status::PENDING_ERROR;
            return RetCode::ERROR;

        } else if (2 == ret) {
            // special event
            return ret_code_;
        }

        if (Status::FINISHED == status_) {
            return RetCode::MESSAGE_READY;
        } else if (Status::CLOSED == status_) {
            return RetCode::PEER_CLOSED;
        }
    }
}

static bool checkHandshakeRequestValid(const HttpRequest &request)
{
    // check header 'Host'
    if (request.hasHeader("Host") == false) {
        return false;
    }
    // check header 'Upgrade'
    if (request.headerContain("Upgrade", "websocket") == false) {
        return false;
    }
    // check header 'Connection'
    if (request.headerContain("Connection", "Upgrade") == false) {
        return false;
    }
    // check header 'Sec-WebSocket-Key'
    const std::string &sec_key = request.getHeader("Sec-WebSocket-Key");
    if (sec_key.size() != 24) {
        return false;
    }
    // check header 'Sec-WebSocket-Version'
    if (request.headerEqual("Sec-WebSocket-Version", "13") == false) {
        return false;
    }

    return true;
}

static void sendErrorResponse(TcpService *tcp_service,
                              TcpService::SocketId socket_id)
{
    static const char http_400[] = "HTTP/1.1 400 Bad Request\r\n\r\n";
    tcp_service->sendMessage(socket_id, http_400, sizeof(http_400) - 1);
}

static bool sendHandshakeResponse(TcpService *tcp_service,
    TcpService::SocketId socket_id,
    const HttpRequest &request,
    const WebSocketProtocol::HeaderMap &handshake_headers)
{
    const std::string &sec_key = request.getHeader("Sec-WebSocket-Key");

    HttpResponse response;
    response.setVersion(HttpMessage::Version::HTTP_1_1);
    response.setStatusCode(101);
    response.setReasonPhrase("Switching Protocols");
    response.setHeader("Upgrade", "websocket");
    response.setHeader("Connection", "Upgrade");
    response.setHeader("Sec-WebSocket-Accept",
        codec::base64Encode(codec::sha1Binary(sec_key +
            "258EAFA5-E914-47DA-95CA-C5AB0DC85B11")));

    // set handshake headers
    for (WebSocketProtocol::HeaderMap::const_iterator iter =
            handshake_headers.begin();
         iter != handshake_headers.end(); ++iter) {
        response.setHeader(iter->first, iter->second);
    }

    // process header 'date'
    if (response.hasHeader("Date")) {
        response.setDate();
    }

    DynamicBuffer buffer;
    HttpProtocol::writeMessage(response, &buffer);
    if (tcp_service->sendMessage(socket_id,
            buffer.readBegin(), buffer.readableBytes()) == false) {
        return false;
    }

    return true;
}

int WebSocketProtocol::Impl::readHandshakeRequest(DynamicBuffer *buffer)
{
    HttpProtocol::RetCode::type ret = http_protocol_.recvMessage(buffer);
    if (HttpProtocol::RetCode::WAITING_MORE_DATA == ret) {
        return 0;

    } else if (HttpProtocol::RetCode::MESSAGE_READY == ret) {
        HttpRequest request;
        if (http_protocol_.retrieveRequest(&request) == false) {
            sendErrorResponse(tcp_service_, socket_id_);
            return -1;
        }

        if (checkHandshakeRequestValid(request) == false) {
            sendErrorResponse(tcp_service_, socket_id_);
            return -1;
        }

        if (sendHandshakeResponse(tcp_service_,
                socket_id_, request, handshake_headers_) == false) {
            return -1;
        }

        status_ = Status::CONNECTED;
        ret_code_ = RetCode::CONNECTION_ESTABLISHED;
        return 2;

    } else {
        sendErrorResponse(tcp_service_, socket_id_);
        return -1;
    }
}

static bool checkHandshakeResponseValid( const HttpResponse &response,
    const std::string &sec_key)
{
    // check status code
    if (response.getStatusCode() != 101) {
        return false;
    }

    // check header 'Upgrade'
    if (response.headerContain("Upgrade", "websocket") == false) {
        return false;
    }

    // check header 'Connection'
    if (response.headerContain("Connection", "Upgrade") == false) {
        return false;
    }

    // check header 'Sec-WebSocket-Accept'
    std::string sec_accept = codec::base64Encode(codec::sha1Binary(sec_key +
        "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"));
    if (response.headerEqual("Sec-WebSocket-Accept", sec_accept) == false) {
        return false;
    }

    return true;
}

int WebSocketProtocol::Impl::readHandshakeResponse(DynamicBuffer *buffer)
{
    HttpProtocol::RetCode::type ret = http_protocol_.recvMessage(buffer);
    if (HttpProtocol::RetCode::WAITING_MORE_DATA == ret) {
        return 0;

    } else if (HttpProtocol::RetCode::MESSAGE_READY == ret) {
        HttpResponse response;
        if (http_protocol_.retrieveResponse(&response) == false) {
            return -1;
        }

        if (checkHandshakeResponseValid(response,
                handshake_headers_["Sec-WebSocket-Key"]) == false) {
            return -1;
        }

        status_ = Status::CONNECTED;
        ret_code_ = RetCode::CONNECTION_ESTABLISHED;
        return 2;

    } else {
        return -1;
    }
}

int WebSocketProtocol::Impl::readFrame(DynamicBuffer *buffer)
{
    if (buffer->readableBytes() < 2) {
        return 0;
    }

    const uint8_t *b = (uint8_t *)buffer->readBegin();

    // check RSV1, RSV2, RSV3
    if ((b[0] & 0x70) != 0) {
        return -1;
    }

    const uint8_t *p = b + 2;
    size_t left_bytes = buffer->readableBytes() - 2;

    // get payload length
    uint64_t payload_length = b[1] & 0x7f;
    if (126 == payload_length) {
        // check enough data
        if (left_bytes < 2) {
            return 0;
        }

        payload_length = ntohs(*(uint16_t *)p);

        // the minimal number of bytes MUST be used to encode the length
        if (payload_length < 126) {
            return -1;
        }

        // move forward
        p += 2;
        left_bytes -= 2;

    } else if (127 == payload_length) {
        // check enough data
        if (left_bytes < 8) {
            return 0;
        }

        payload_length = (uint64_t)ntohl(*(uint32_t *)(p + 4)) +
                        ((uint64_t)ntohl(*(uint32_t *)p) << 32);

        // the minimal number of bytes MUST be used to encode the length
        if (payload_length < 0xffff) {
            return -1;
        }

        // move forward
        p += 8;
        left_bytes -= 8;
    }

    // get mask key
    bool mask = b[1] & 0x80;
    const uint8_t *mask_key = NULL;
    if (mask) {
        // A server MUST NOT mask any frames that it sends to the client
        if (is_client_) {
            return -1;
        }
        // check enough data
        if (left_bytes < 4) {
            return 0;
        }
        
        mask_key = p;

        // move forward
        p += 4;
        left_bytes -= 4;
    }

    // get payload
    if (payload_length > 0) {
        // check enough data for payload
        if (left_bytes < payload_length) {
            return 0;
        }
        // copy payload
        message_.reserveWritableBytes(payload_length);
        ::memcpy(message_.writeBegin(), p, payload_length);
        // do masking
        if (mask) {
            for (size_t i = 0; i < payload_length; ++i) {
                *(message_.writeBegin() + i) ^= mask_key[i & 0x03];
            }
        }
        message_.write(payload_length);
        // move forward
        p += payload_length;
        left_bytes -= payload_length;
    }

    buffer->read(p - b);

    // process frame
    bool fin = b[0] & 0x80;
    int opcode = b[0] & 0x0f;

    // not fin
    if (!fin) {
        if (opcode != 0x0) {
            last_op_code_ = opcode;
        }
        return 0;
    }

    // fin
    if (0x0 == opcode) {
        opcode = last_op_code_;
        last_op_code_ = -1;
    }

    if (0x1 == opcode || 0x2 == opcode) {
        // data frame
        status_ = Status::FINISHED;
        return 1;

    } else if (0x8 == opcode) {
        // close frame
        sendCloseFrame();
        status_ = Status::CLOSED;
        message_.clear();
        return 1;

    } else if (0x9 == opcode) {
        // ping frame
        message_.clear();
        sendPongFrame();
        ret_code_ = RetCode::PING_FRAME;
        return 2;

    } else if (0xa == opcode) {
        // pong frame
        message_.clear();
        ret_code_ = RetCode::PONG_FRAME;
        return 2;

    } else {
        return -1;
    }
}

bool WebSocketProtocol::Impl::retrieveMessage(DynamicBuffer *message)
{
    if (status_ != Status::FINISHED) {
        return false;
    }

    message->swap(message_);
    message_.clear();

    status_ = Status::CONNECTED;

    return true;
}

bool WebSocketProtocol::Impl::sendMessage(const char *buffer, size_t size)
{
    if (status_ != Status::CONNECTED) {
        return false;
    }
    if (close_frame_sent_) {
        return false;
    }

    DynamicBuffer message;

    message.reserveWritableBytes(2);
    // FIN = 1, RSV1~RSV3 = 0, opcode = 0x2
    message.writeBegin()[0] = 0x82;
    message.write(1);

    // mask flag
    message.writeBegin()[0] = (is_client_) ? 0x80 : 0x0;

    // payload length
    if (size < 126) {
        message.writeBegin()[0] |= size;
        message.write(1);

    } else if (size < 65535) {
        message.writeBegin()[0] |= 126;
        message.write(1);
        message.reserveWritableBytes(2);
        *(uint16_t *)message.writeBegin() = htons(size);
        message.write(2);

    } else {
        message.writeBegin()[0] |= 127;
        message.write(1);
        message.reserveWritableBytes(8);
        *(uint32_t *)message.writeBegin() = htonl(size >> 32);
        *(uint32_t *)(message.writeBegin() + 4) = htonl(size & 0xffffffff);
        message.write(8);
    }

    // mask key
    const uint8_t *mask_key = NULL;
    if (is_client_) {
        message.reserveWritableBytes(4);
        for (size_t i = 0; i < 4; ++i) {
            message.writeBegin()[i] = random_generator_->nextInt(256);
        }
        mask_key = (uint8_t *)message.writeBegin();
        message.write(4);
    }

    // payload
    message.reserveWritableBytes(size);
    ::memcpy(message.writeBegin(), buffer, size);
    // do masking
    if (mask_key != NULL) {
        for (size_t i = 0; i < size; ++i) {
            message.writeBegin()[i] ^= mask_key[i & 0x03];
        }
    }
    message.write(size);

    if (tcp_service_->sendMessage(socket_id_,
            message.readBegin(), message.readableBytes()) == false) {
        return false;
    }

    return true;
}

bool WebSocketProtocol::Impl::sendCloseFrame()
{
    // FIN = 1, RSV1~RSV3 = 0, opcode = 0x8, payload_length = 0
    static const uint8_t client_frame[] = { 0x88, 0x80, 0x0, 0x0, 0x0, 0x0};
    static const uint8_t server_frame[] = { 0x88, 0x0 };

    if (status_ != Status::CONNECTED) {
        return false;
    }
    if (close_frame_sent_) {
        return false;
    }

    close_frame_sent_ = true;

    if (is_client_) {
        return tcp_service_->sendMessage(socket_id_,
            (const char *)client_frame, sizeof(client_frame));
    } else {
        return tcp_service_->sendMessage(socket_id_,
            (const char *)server_frame, sizeof(server_frame));
    }
}

bool WebSocketProtocol::Impl::sendPingFrame()
{
    // FIN = 1, RSV1~RSV3 = 0, opcode = 0x9, payload_length = 0
    static const uint8_t client_frame[] = { 0x89, 0x80, 0x0, 0x0, 0x0, 0x0};
    static const uint8_t server_frame[] = { 0x89, 0x0 };

    if (status_ != Status::CONNECTED) {
        return false;
    }
    if (close_frame_sent_) {
        return false;
    }

    if (is_client_) {
        return tcp_service_->sendMessage(socket_id_,
            (const char *)client_frame, sizeof(client_frame));
    } else {
        return tcp_service_->sendMessage(socket_id_,
            (const char *)server_frame, sizeof(server_frame));
    }

    return false;
}

bool WebSocketProtocol::Impl::sendPongFrame()
{
    // FIN = 1, RSV1~RSV3 = 0, opcode = 0xa, payload_length = 0
    static const uint8_t client_frame[] = { 0x8a, 0x80, 0x0, 0x0, 0x0, 0x0};
    static const uint8_t server_frame[] = { 0x8a, 0x0 };

    if (status_ != Status::CONNECTED) {
        return false;
    }
    if (close_frame_sent_) {
        return false;
    }

    if (is_client_) {
        return tcp_service_->sendMessage(socket_id_,
            (const char *)client_frame, sizeof(client_frame));
    } else {
        return tcp_service_->sendMessage(socket_id_,
            (const char *)server_frame, sizeof(server_frame));
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////
WebSocketProtocol::WebSocketProtocol() :
    pimpl_(new Impl())
{
}

WebSocketProtocol::~WebSocketProtocol()
{
}

WebSocketProtocol::Status::type WebSocketProtocol::getStatus() const
{
    return pimpl_->getStatus();
}

void WebSocketProtocol::setHandshakeHeader(const std::string &key,
                                           const std::string &value)
{
    return pimpl_->setHandshakeHeader(key, value);
}

bool WebSocketProtocol::startAsClient(
    TcpService &tcp_service, TcpService::SocketId socket_id,
    Random &random_generator, const char *request_uri)
{
    return pimpl_->startAsClient(tcp_service, socket_id,
                                 random_generator, request_uri);
}

bool WebSocketProtocol::startAsServer(
    TcpService &tcp_service, TcpService::SocketId socket_id)
{
    return pimpl_->startAsServer(tcp_service, socket_id);
}

WebSocketProtocol::RetCode::type WebSocketProtocol::recvMessage(
    DynamicBuffer *buffer)
{
    return pimpl_->recvMessage(buffer);
}

bool WebSocketProtocol::retrieveMessage(DynamicBuffer *message)
{
    return pimpl_->retrieveMessage(message);
}

bool WebSocketProtocol::sendMessage(const char *buffer, size_t size)
{
    return pimpl_->sendMessage(buffer, size);
}

bool WebSocketProtocol::sendCloseFrame()
{
    return pimpl_->sendCloseFrame();
}

bool WebSocketProtocol::sendPingFrame()
{
    return pimpl_->sendPingFrame();
}

} // namespace protocol
} // namespace brickred

#ifndef BRICKRED_PROTOCOL_HTTP_PROTOCOL_H
#define BRICKRED_PROTOCOL_HTTP_PROTOCOL_H

#include <brickred/class_util.h>
#include <brickred/unique_ptr.h>

namespace brickred {

class DynamicBuffer;

} // namespace brickred

namespace brickred {
namespace protocol {

class HttpMessage;
class HttpRequest;
class HttpResponse;

class HttpProtocol {
public:
    struct Status {
        enum type {
            READING_START_LINE = 0,
            READING_HEADER,
            READING_BODY,
            FINISHED,
            PENDING_ERROR,
            MAX
        };
    };

    struct RetCode {
        enum type {
            ERROR = -1,
            WAITING_MORE_DATA = 0,
            MESSAGE_READY = 1
        };
    };

    HttpProtocol();
    ~HttpProtocol();
    void reset();

    Status::type getStatus() const;
    RetCode::type recvMessage(DynamicBuffer *buffer);
    bool retrieveRequest(HttpRequest *request);
    bool retrieveResponse(HttpResponse *response);

    static void writeMessage(const HttpMessage &message,
                             DynamicBuffer *buffer);

private:
    BRICKRED_NONCOPYABLE(HttpProtocol)

    class Impl;
    UniquePtr<Impl> pimpl_;
};

} // namespace protocol
} // namespace brickred

#endif

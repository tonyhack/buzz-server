#ifndef BRICKRED_PROTOCOL_HTTP_RESPONSE_H
#define BRICKRED_PROTOCOL_HTTP_RESPONSE_H

#include <string>

#include <brickred/protocol/http_message.h>

namespace brickred {
namespace protocol {

class HttpResponse : public HttpMessage {
public:
    HttpResponse();
    virtual ~HttpResponse();
    void swap(HttpResponse &other);

    int getStatusCode() const { return status_code_; }
    const std::string &getReasonPhrase() const { return reason_phrase_; }

    void setStatusCode(int status_code);
    void setReasonPhrase(const std::string &reason_phrase);

private:
    int status_code_;
    std::string reason_phrase_;
};

} // namespace protocol
} // namespace brickred

#endif

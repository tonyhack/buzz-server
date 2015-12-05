#include <brickred/protocol/http_response.h>

#include <algorithm>

namespace brickred {
namespace protocol {

HttpResponse::HttpResponse() :
    status_code_(0)
{
    message_type_ = MessageType::RESPONSE;
}

HttpResponse::~HttpResponse()
{
}

void HttpResponse::swap(HttpResponse &other)
{
    HttpMessage::swap(other);
    std::swap(status_code_, other.status_code_);
    reason_phrase_.swap(other.reason_phrase_);
}

void HttpResponse::setStatusCode(int status_code)
{
    status_code_ = status_code;
}

void HttpResponse::setReasonPhrase(const std::string &reason_phrase)
{
    reason_phrase_ = reason_phrase;
}

} // namespace protocol
} // namespace brickred

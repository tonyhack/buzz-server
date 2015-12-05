#include <brickred/protocol/http_request.h>

#include <algorithm>

namespace brickred {
namespace protocol {

static const std::string s_cstr_get("GET");
static const std::string s_cstr_post("POST");
static const std::string s_cstr_unknown("UNKNOWN");

HttpRequest::HttpRequest() :
    method_(Method::UNKNOWN)
{
    message_type_ = MessageType::REQUEST;
}

HttpRequest::~HttpRequest()
{
}

void HttpRequest::swap(HttpRequest &other)
{
    HttpMessage::swap(other);
    std::swap(method_, other.method_);
    request_uri_.swap(other.request_uri_);
}

void HttpRequest::setMethod(Method::type method)
{
    method_ = method;
}

void HttpRequest::setRequestUri(const std::string &request_uri)
{
    request_uri_ = request_uri;
}

HttpRequest::Method::type HttpRequest::MethodStrToEnum(
    const std::string &method_str)
{
    if (method_str == s_cstr_get) {
        return Method::GET;
    } else if (method_str == s_cstr_post) {
        return Method::POST;
    } else {
        return Method::UNKNOWN;
    }
}

const std::string &HttpRequest::MethodEnumToStr(Method::type method_enum)
{

    if (Method::GET == method_enum) {
        return s_cstr_get;
    } else if (Method::POST == method_enum) {
        return s_cstr_post;
    } else {
        return s_cstr_unknown;
    }
}

} // namespace protocol
} // namespace brickred

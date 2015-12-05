#include <brickred/protocol/http_message.h>

#include <algorithm>

#include <brickred/timestamp.h>

namespace brickred {
namespace protocol {

static const std::string s_cstr_empty_string;
static const std::string s_cstr_http_1_1("HTTP/1.1");
static const std::string s_cstr_http_1_0("HTTP/1.0");
static const std::string s_cstr_unknown("UNKNOWN");

HttpMessage::HttpMessage() :
    version_(Version::UNKNOWN)
{
}

HttpMessage::~HttpMessage()
{
}

void HttpMessage::swap(HttpMessage &other)
{
    std::swap(message_type_, other.message_type_);
    std::swap(version_, other.version_);
    headers_.swap(other.headers_);
    body_.swap(other.body_);
}

const std::string &HttpMessage::getHeader(const std::string &key) const
{
    HeaderMap::const_iterator iter = headers_.find(key);
    if (iter == headers_.end()) {
        return s_cstr_empty_string;
    } else {
        return iter->second;
    }
}

bool HttpMessage::hasHeader(const std::string &key) const
{
    return headers_.find(key) != headers_.end();
}

bool HttpMessage::headerEqual(const std::string &key,
                              const std::string &value) const
{
    return string_util::stricmp(getHeader(key), value);
}

bool HttpMessage::headerContain(const std::string &key,
                                const std::string &value) const
{
    return string_util::toLower(getHeader(key)).find(
               string_util::toLower(value)) != std::string::npos;
}

void HttpMessage::setVersion(Version::type version)
{
    version_ = version;
}

void HttpMessage::setHeader(const std::string &key, const std::string &value)
{
    headers_[key] = value;
}

void HttpMessage::removeHeader(const std::string &key)
{
    headers_.erase(key);
}

void HttpMessage::setBody(const char *buffer, size_t size)
{
    body_.assign(buffer, size);
}

void HttpMessage::setBody(const std::string &body)
{
    body_ = body;
}

bool HttpMessage::isConnectionKeepAlive() const
{
    return headerEqual("Connection", "Keep-Alive");
}

void HttpMessage::setConnectionKeepAlive()
{
    setHeader("Connection", "Keep-Alive");
}

void HttpMessage::setConnectionClose()
{
    setHeader("Connection", "Close");
}

void HttpMessage::setDate(time_t now)
{
    if (0 == now) {
        Timestamp ts;
        ts.setNow();
        now = ts.getSecond();
    }

    char date_string[256];
    Timestamp::format(date_string, sizeof(date_string),
                      "%a, %d %b %Y %H:%M:%S %Z", now);
    setHeader("Date", date_string);
}

HttpMessage::Version::type HttpMessage::VersionStrToEnum(
    const std::string &version_str)
{
    if (version_str == s_cstr_http_1_1) {
        return Version::HTTP_1_1;
    } else if (version_str == s_cstr_http_1_0) {
        return Version::HTTP_1_0;
    } else {
        return Version::UNKNOWN;
    }
}

const std::string &HttpMessage::VersionEnumToStr(Version::type version_enum)
{

    if (Version::HTTP_1_1 == version_enum) {
        return s_cstr_http_1_1;
    } else if (Version::HTTP_1_0 == version_enum) {
        return s_cstr_http_1_0;
    } else {
        return s_cstr_unknown;
    }
}

} // namespace protocol
} // namespace brickred

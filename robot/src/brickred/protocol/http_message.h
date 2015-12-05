#ifndef BRICKRED_PROTOCOL_HTTP_MESSAGE_H
#define BRICKRED_PROTOCOL_HTTP_MESSAGE_H

#include <ctime>
#include <cstddef>
#include <map>
#include <string>

#include <brickred/string_util.h>

namespace brickred {
namespace protocol {

class HttpMessage {
public:
    struct Version {
        enum type {
            UNKNOWN  = 0,
            HTTP_1_0,
            HTTP_1_1
        };
    };

    struct MessageType {
        enum type {
            UNKNOWN = 0,
            REQUEST,
            RESPONSE
        };
    };

    typedef std::map<std::string, std::string,
                     string_util::CaseInsensitiveLess> HeaderMap;

    HttpMessage();
    virtual ~HttpMessage() = 0;
    void swap(HttpMessage &other);

    MessageType::type getMessageType() const { return message_type_; }
    Version::type getVersion() const { return version_; }
    const HeaderMap &getHeaders() const { return headers_; }
    const std::string &getHeader(const std::string &key) const;
    bool hasHeader(const std::string &key) const;
    bool headerEqual(const std::string &key,
                     const std::string &value) const;
    bool headerContain(const std::string &key,
                       const std::string &value) const;
    const std::string &getBody() const { return body_; }

    void setVersion(Version::type version);
    void setHeader(const std::string &key, const std::string &value);
    void removeHeader(const std::string &key);
    void setBody(const char *buffer, size_t size);
    void setBody(const std::string &body);

    bool isConnectionKeepAlive() const;
    void setConnectionKeepAlive();
    void setConnectionClose();
    void setDate(time_t now = 0);

    static Version::type VersionStrToEnum(const std::string &version_str);
    static const std::string &VersionEnumToStr(Version::type version_enum);

protected:
    MessageType::type message_type_;
    Version::type version_;
    HeaderMap headers_;
    std::string body_;
};

} // namespace protocol
} // namespace brickred

#endif

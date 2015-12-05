#include <brickred/socket_address.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstddef>
#include <cstring>

namespace brickred {

class SocketAddress::Impl {
public:
    typedef SocketAddress::Protocol Protocol;
    typedef union native_address_tag {
        struct sockaddr_in ipv4_addr_;
        struct sockaddr_in6 ipv6_addr_;
    } NativeAddress;

    Impl();
    Impl(const std::string &ip, uint16_t port);
    ~Impl();

    const std::string &getIp() const { return ip_; }
    uint16_t getPort() const { return port_; }
    Protocol::type getProtocol() const;
    void setIp(const std::string &ip);
    void setPort(uint16_t port);

    const void *getNativeAddress() const;
    size_t getNativeAddressSize() const { return native_addr_size_; }
    bool setNativeAddress(const void *native_addr);

public:
    void translateToNativeAddress() const;

private:
    std::string ip_;
    uint16_t port_;
    mutable Protocol::type protocol_;
    mutable NativeAddress native_addr_;
    mutable size_t native_addr_size_;
    mutable bool translated_;
};

SocketAddress::Impl::Impl() :
    port_(0), protocol_(Protocol::UNKNOWN),
    native_addr_size_(0), translated_(false)
{
}

SocketAddress::Impl::Impl(const std::string &ip, uint16_t port) :
    ip_(ip), port_(port), protocol_(Protocol::UNKNOWN),
    native_addr_size_(0), translated_(false)
{
}

SocketAddress::Impl::~Impl()
{
}

SocketAddress::Impl::Protocol::type SocketAddress::Impl::getProtocol() const
{
    if (!translated_) {
        translateToNativeAddress();
    }

    return protocol_;
}

void SocketAddress::Impl::setIp(const std::string &ip)
{
    ip_ = ip;
    translated_ = false;
}

void SocketAddress::Impl::setPort(uint16_t port)
{
    port_ = port;
}

const void *SocketAddress::Impl::getNativeAddress() const
{
    if (!translated_) {
        translateToNativeAddress();
    }

    if (Protocol::IP_V4 == protocol_) {
        return &native_addr_.ipv4_addr_;
    } else if (Protocol::IP_V6 == protocol_) {
        return &native_addr_.ipv6_addr_;
    } else {
        return NULL;
    }
}

bool SocketAddress::Impl::setNativeAddress(const void *native_addr)
{
    struct sockaddr *sock_addr = (struct sockaddr *)native_addr;
    char ip[1024];
    uint64_t port = 0;
    Protocol::type protocol = Protocol::UNKNOWN;
    size_t native_addr_size = 0;

    if (AF_INET == sock_addr->sa_family) {
        struct sockaddr_in *sock_addr4 = (struct sockaddr_in *)native_addr;
        if (::inet_ntop(AF_INET, &sock_addr4->sin_addr,
                        ip, sizeof(ip)) == NULL) {
            return false;
        }
        port = ntohs(sock_addr4->sin_port);
        protocol = Protocol::IP_V4;
        native_addr_size = sizeof(struct sockaddr_in);

    } else if (AF_INET6 == sock_addr->sa_family) {
        struct sockaddr_in6 *sock_addr6 = (struct sockaddr_in6 *)native_addr;
        if (::inet_ntop(AF_INET6, &sock_addr6->sin6_addr,
                        ip, sizeof(ip)) == NULL) {
            return false;
        }
        port = ntohs(sock_addr6->sin6_port);
        protocol = Protocol::IP_V6;
        native_addr_size = sizeof(struct sockaddr_in6);

    } else {
        return false;
    }

    ip_ = ip;
    port_ = port;
    protocol_ = protocol;
    native_addr_size_ = native_addr_size;
    memcpy(&native_addr_, native_addr, native_addr_size);
    translated_ = true;

    return true;
}

void SocketAddress::Impl::translateToNativeAddress() const
{
    translated_ = true;

    if (ip_.find(".") != ip_.npos) {
        // try ipv4 convert
        ::memset(&native_addr_.ipv4_addr_.sin_addr, 0,
                 sizeof(struct sockaddr_in));
        if (::inet_pton(AF_INET, ip_.c_str(),
                        &native_addr_.ipv4_addr_.sin_addr) != 1) {
            return;
        }
        native_addr_.ipv4_addr_.sin_family = AF_INET;
        native_addr_.ipv4_addr_.sin_port = htons(port_);
        native_addr_size_ = sizeof(struct sockaddr_in);
        protocol_ = Protocol::IP_V4;

    } else if (ip_.find(":") != ip_.npos) {
        // try ipv6 convert
        ::memset(&native_addr_.ipv6_addr_.sin6_addr, 0,
                 sizeof(struct sockaddr_in6));
        if (::inet_pton(AF_INET6, ip_.c_str(),
                        &native_addr_.ipv6_addr_.sin6_addr) != 1) {
            return;
        }
        native_addr_.ipv6_addr_.sin6_family = AF_INET6;
        native_addr_.ipv6_addr_.sin6_port = htons(port_);
        native_addr_size_ = sizeof(struct sockaddr_in6);
        protocol_ = Protocol::IP_V6;
    }
}

///////////////////////////////////////////////////////////////////////////////
SocketAddress::SocketAddress() :
    pimpl_(new Impl())
{
}

SocketAddress::SocketAddress(const std::string &ip, uint16_t port) :
    pimpl_(new Impl(ip, port))
{
}

SocketAddress::SocketAddress(const SocketAddress &copy) :
    pimpl_(new Impl(*copy.pimpl_))
{
}

SocketAddress::~SocketAddress()
{
}

SocketAddress &SocketAddress::operator=(const SocketAddress &rhs)
{
    if (this == &rhs) {
        return *this;
    }

    *pimpl_ = *rhs.pimpl_;

    return *this;
}

const std::string &SocketAddress::getIp() const
{
    return pimpl_->getIp();
}

uint16_t SocketAddress::getPort() const
{
    return pimpl_->getPort();
}

SocketAddress::Protocol::type SocketAddress::getProtocol() const
{
    return pimpl_->getProtocol();
}

void SocketAddress::setIp(const std::string &ip)
{
    return pimpl_->setIp(ip);
}

void SocketAddress::setPort(uint16_t port)
{
    return pimpl_->setPort(port);
}

const void *SocketAddress::getNativeAddress() const
{
    return pimpl_->getNativeAddress();
}

size_t SocketAddress::getNativeAddressSize() const
{
    return pimpl_->getNativeAddressSize();
}

bool SocketAddress::setNativeAddress(const void *native_addr)
{
    return pimpl_->setNativeAddress(native_addr);
}

bool SocketAddress::getAddressByDomain(const std::string &domain,
    std::vector<SocketAddress> *addr_list)
{
    struct addrinfo *res = NULL;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int ret = ::getaddrinfo(domain.c_str(), NULL, &hints, &res);
    if (ret != 0) {
        return false;
    }

    for (const struct addrinfo *p = res; p != NULL; p = p->ai_next) {
        SocketAddress addr;
        addr.setNativeAddress(p->ai_addr);
        addr_list->push_back(addr);
    }

    ::freeaddrinfo(res);

    return true;
}

bool SocketAddress::ipV4TextToBin(const std::string &text, uint32_t *binary)
{
    struct in_addr addr;
    ::memset(&addr, 0, sizeof(addr));
    if (::inet_pton(AF_INET, text.c_str(), &addr) != 1) {
        return false;
    }

    *binary = ntohl(addr.s_addr);

    return true;
}

void SocketAddress::ipV4BinToText(uint32_t binary, std::string *text)
{
    struct in_addr addr;
    ::memset(&addr, 0, sizeof(addr));
    addr.s_addr = htonl(binary);

    char ip[1024];
    if (::inet_ntop(AF_INET, &addr, ip, sizeof(ip)) == NULL) {
        return;
    }

    *text = ip;
}

std::string SocketAddress::ipV4BinToText(uint32_t binary)
{
    std::string text;
    ipV4BinToText(binary, &text);
    return text;
}

bool SocketAddress::ipV6TextToBin(const std::string &text, uint8_t binary[16])
{
    struct in6_addr addr;
    ::memset(&addr, 0, sizeof(addr));
    if (::inet_pton(AF_INET6, text.c_str(), &addr) != 1) {
        return false;
    }

    for (int i = 0; i < 16; ++i) {
        binary[i] = addr.s6_addr[i];
    }

    return true;
}

void SocketAddress::ipV6BinToText(const uint8_t binary[16], std::string *text)
{
    struct in6_addr addr;
    ::memset(&addr, 0, sizeof(addr));
    for (int i = 0; i < 16; ++i) {
        addr.s6_addr[i] = binary[i];
    }

    char ip[1024];
    if (::inet_ntop(AF_INET, &addr, ip, sizeof(ip)) == NULL) {
        return;
    }

    *text = ip;
}

std::string SocketAddress::ipV6BinToText(const uint8_t binary[16])
{
    std::string text;
    ipV6BinToText(binary, &text);
    return text;
}

} // namespace brickred

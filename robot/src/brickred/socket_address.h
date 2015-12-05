#ifndef BRICKRED_SOCKET_ADDRESS_H
#define BRICKRED_SOCKET_ADDRESS_H

#include <stdint.h>
#include <string>
#include <vector>

#include <brickred/class_util.h>
#include <brickred/unique_ptr.h>

namespace brickred {

class SocketAddress {
public:
    struct Protocol {
        enum type {
            UNKNOWN = 0,
            IP_V4,
            IP_V6
        };
    };

    SocketAddress();
    SocketAddress(const std::string &ip, uint16_t port);
    SocketAddress(const SocketAddress &copy);
    ~SocketAddress();

    SocketAddress &operator=(const SocketAddress &rhs);

    const std::string &getIp() const;
    uint16_t getPort() const;
    Protocol::type getProtocol() const;
    void setIp(const std::string &ip);
    void setPort(uint16_t port);

    const void *getNativeAddress() const;
    size_t getNativeAddressSize() const;
    bool setNativeAddress(const void *native_addr);

    static bool getAddressByDomain(const std::string &domain,
        std::vector<SocketAddress> *addr_list);

    static bool ipV4TextToBin(const std::string &text, uint32_t *binary);
    static void ipV4BinToText(uint32_t binary, std::string *text);
    static std::string ipV4BinToText(uint32_t binary);
    static bool ipV6TextToBin(const std::string &text, uint8_t binary[16]);
    static void ipV6BinToText(const uint8_t binary[16], std::string *text);
    static std::string ipV6BinToText(const uint8_t binary[16]);

private:
    class Impl;
    UniquePtr<Impl> pimpl_;
};

} // namespace brickred

#endif

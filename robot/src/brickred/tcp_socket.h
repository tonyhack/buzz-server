#ifndef BRICKRED_TCP_SOCKET_H
#define BRICKRED_TCP_SOCKET_H

#include <cstddef>

#include <brickred/io_device.h>
#include <brickred/socket_address.h>

namespace brickred {

class TcpSocket : public IODevice {
public:
    TcpSocket();
    virtual ~TcpSocket();

    bool open(SocketAddress::Protocol::type protocol);
    void close();
    bool bind(const SocketAddress &addr);
    bool connect(const SocketAddress &addr);
    bool listen(int backlog);
    bool accept(TcpSocket *peer);

    bool getLocalAddress(SocketAddress *addr) const;
    bool getPeerAddress(SocketAddress *addr) const;

    int readableBytes() const;
    int recv(char *buffer, size_t size);
    int send(const char *buffer, size_t size);

    // -*- tcp options -*-
    int getSocketError();
    bool setReuseAddr();
    bool setTcpNoDelay();

    // -*- builder methods -*-
    // open()
    // setReuseAddr(), setTcpNoDelay()
    // connect()
    bool activeOpen(const SocketAddress &remote_addr);
    // activeOpen()
    // setNonblock()
    bool activeOpenNonblock(const SocketAddress &remote_addr);
    // open()
    // setReuseAddr(), setTcpNoDelay()
    // bind(), listen(128)
    bool passiveOpen(const SocketAddress &local_addr);
    // passiveOpen()
    // setNonblock()
    bool passiveOpenNonblock(const SocketAddress &local_addr);
    // accept()
    // setTcpNoDelay()
    // setNonblock()
    bool acceptNonblock(TcpSocket *peer);
};

} // namespace brickred

#endif

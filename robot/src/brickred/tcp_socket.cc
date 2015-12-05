#include <brickred/tcp_socket.h>

#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <cerrno>

#include <brickred/socket_address.h>

namespace brickred {

TcpSocket::TcpSocket()
{
}

TcpSocket::~TcpSocket()
{
    close();
}

bool TcpSocket::open(SocketAddress::Protocol::type protocol)
{
    if (fd_ != -1) {
        close();
    }

    if (SocketAddress::Protocol::IP_V4 == protocol) {
        fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
        if (-1 == fd_) {
            return false;
        }
    } else if (SocketAddress::Protocol::IP_V6 == protocol) {
        fd_ = ::socket(AF_INET6, SOCK_STREAM, 0);
        if (-1 == fd_) {
            return false;
        }
    } else {
        errno = EAFNOSUPPORT;
        return false;
    }

    if (setCloseOnExec() == false) {
        close();
    }

    return true;
}

void TcpSocket::close()
{
    detachIOService();

    if (fd_ != -1) {
        ::close(fd_);
        fd_ = -1;
    }
}

bool TcpSocket::connect(const SocketAddress &addr)
{
    if (addr.getNativeAddress() == NULL) {
        errno = EAFNOSUPPORT;
        return false;
    }

    if (::connect(fd_, (const struct sockaddr *)addr.getNativeAddress(),
                  addr.getNativeAddressSize()) != 0) {
        return false;
    }

    return true;
}

bool TcpSocket::bind(const SocketAddress &addr)
{
    if (addr.getNativeAddress() == NULL) {
        errno = EAFNOSUPPORT;
        return false;
    }

    if (::bind(fd_, (const struct sockaddr *)addr.getNativeAddress(),
               addr.getNativeAddressSize()) != 0) {
        return false;
    }

    return true;
}

bool TcpSocket::listen(int backlog)
{
    if (::listen(fd_, backlog) != 0) {
        return false;
    }

    return true;
}

bool TcpSocket::accept(TcpSocket *peer)
{
    int sock_fd = ::accept(fd_, NULL, NULL);
    if (-1 == sock_fd) {
        return false;
    }

    peer->setDescriptor(sock_fd);

    if (peer->setCloseOnExec() == false) {
        peer->close();
        return false;
    }

    return true;
}

bool TcpSocket::getLocalAddress(SocketAddress *addr) const
{
    struct sockaddr sock_addr;
    socklen_t addr_len = sizeof(sock_addr);
    if (::getsockname(fd_, &sock_addr, &addr_len) != 0) {
        return false;
    }

    return addr->setNativeAddress(&sock_addr);
}

bool TcpSocket::getPeerAddress(SocketAddress *addr) const
{
    struct sockaddr sock_addr;
    socklen_t addr_len = sizeof(sock_addr);
    if (::getpeername(fd_, (struct sockaddr *)&sock_addr, &addr_len) != 0) {
        return false;
    }

    return addr->setNativeAddress(&sock_addr);
}

int TcpSocket::readableBytes() const
{
    int readable_bytes;

    if (ioctl(fd_, FIONREAD, &readable_bytes) == -1) {
      return -1;
    }

    return readable_bytes;
}

int TcpSocket::recv(char *buffer, size_t size)
{
    return ::recv(fd_, buffer, size, 0);
}

int TcpSocket::send(const char *buffer, size_t size)
{
    return ::send(fd_, buffer, size, MSG_NOSIGNAL);
}

int TcpSocket::getSocketError()
{
    int opt = 0;
    socklen_t opt_len = sizeof(opt);
    if (::getsockopt(fd_, SOL_SOCKET, SO_ERROR, &opt, &opt_len) != 0) {
        return errno;
    } else {
        return opt;
    }
}

bool TcpSocket::setReuseAddr()
{
    int opt = 1;
    if (::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        return false;
    }

    return true;
}

bool TcpSocket::setTcpNoDelay()
{
    int opt = 1;
    if (::setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) != 0) {
        return false;
    }

    return true;
}

bool TcpSocket::activeOpen(const SocketAddress &remote_addr)
{
    if (open(remote_addr.getProtocol()) == false) {
        return false;
    }
    if (setReuseAddr() == false ||
        setTcpNoDelay() == false ||
        connect(remote_addr) == false) {
        close();
        return false;
    }

    return true;
}

bool TcpSocket::activeOpenNonblock(const SocketAddress &remote_addr)
{
    if (activeOpen(remote_addr) == false) {
        return false;
    }
    if (setNonblock() == false) {
        close();
        return false;
    }

    return true;
}

bool TcpSocket::passiveOpen(const SocketAddress &local_addr)
{
    if (open(local_addr.getProtocol()) == false) {
        return false;
    }
    if (setReuseAddr() == false ||
        setTcpNoDelay() == false ||
        bind(local_addr) == false ||
        listen(128) == false) {
        close();
        return false;
    }

    return true;
}

bool TcpSocket::passiveOpenNonblock(const SocketAddress &local_addr)
{
    if (passiveOpen(local_addr) == false) {
        return false;
    }
    if (setNonblock() == false) {
        close();
        return false;
    }

    return true;
}

bool TcpSocket::acceptNonblock(TcpSocket *peer)
{
    if (accept(peer) == false) {
        return false;
    }
    if (peer->setTcpNoDelay() == false ||
        peer->setNonblock() == false) {
        peer->close();
        return false;
    }

    return true;
}

} // namespace brickred

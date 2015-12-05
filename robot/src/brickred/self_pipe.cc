#include <brickred/self_pipe.h>

#include <fcntl.h>
#include <unistd.h>

namespace brickred {

SelfPipe::SelfPipe() : fd1_(-1)
{
}

SelfPipe::~SelfPipe()
{
    close();
}

bool SelfPipe::open()
{
    if (fd_ != -1) {
        close();
    }

    int pipefd[2];
    if (::pipe(pipefd) != 0) {
        return false;
    }

    fd_ = pipefd[0];
    fd1_ = pipefd[1];

    return true;
}

void SelfPipe::close()
{
    detachIOService();

    if (fd_ != -1) {
        ::close(fd_);
        fd_ = -1;
    }
    if (fd1_ != -1) {
        ::close(fd1_);
        fd_ = -1;
    }
}

int SelfPipe::read(char *buffer, size_t size)
{
    return ::read(fd_, buffer, size);
}

int SelfPipe::write(const char *buffer, size_t size)
{
    return ::write(fd1_, buffer, size);
}

bool SelfPipe::setNonblock()
{
    int flags = ::fcntl(fd_, F_GETFL, 0);
    if (::fcntl(fd_, F_SETFL, flags | O_NONBLOCK) != 0) {
        return false;
    }
    flags = ::fcntl(fd1_, F_GETFL, 0);
    if (::fcntl(fd1_, F_SETFL, flags | O_NONBLOCK) != 0) {
        return false;
    }

    return true;
}

bool SelfPipe::setCloseOnExec()
{
    int flags = ::fcntl(fd_, F_GETFD, 0);
    if (::fcntl(fd_, F_SETFD, flags | FD_CLOEXEC) != 0) {
        return false;
    }
    flags = ::fcntl(fd1_, F_GETFD, 0);
    if (::fcntl(fd1_, F_SETFD, flags | FD_CLOEXEC) != 0) {
        return false;
    }

    return true;
}

} // namespace brickred

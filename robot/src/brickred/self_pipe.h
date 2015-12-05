#ifndef BRICKRED_SELF_PIPE_H
#define BRICKRED_SELF_PIPE_H

#include <cstddef>

#include <brickred/io_device.h>

namespace brickred {

class SelfPipe : public IODevice {
public:
    SelfPipe();
    virtual ~SelfPipe();

    bool open();
    void close();

    virtual int read(char *buffer, size_t size);
    virtual int write(const char *buffer, size_t size);
    virtual bool setNonblock();
    virtual bool setCloseOnExec();

private:
    DescriptorId fd1_;
};

} // namespace brickred

#endif

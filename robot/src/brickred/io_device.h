#ifndef BRICKRED_IO_DEVICE_H
#define BRICKRED_IO_DEVICE_H

#include <stdint.h>
#include <string>
#include <cstddef>

#include <brickred/class_util.h>
#include <brickred/function.h>

namespace brickred {

class IOService;

class IODevice {
public:
    typedef int DescriptorId;
    typedef Function<void (IODevice *)> ReadCallback;
    typedef Function<void (IODevice *)> WriteCallback;
    typedef Function<void (IODevice *)> ErrorCallback;

    IODevice();
    virtual ~IODevice();

    bool attachIOService(IOService &io_service);
    void detachIOService();
    int64_t getId() const { return id_; }
    void setId(int64_t id) { id_ = id; }
    DescriptorId getDescriptor() const { return fd_; }
    void setDescriptor(DescriptorId fd) { fd_ = fd; }

    const ReadCallback &getReadCallback() const { return read_cb_; }
    const WriteCallback &getWriteCallback() const { return write_cb_; }
    const ErrorCallback &getErrorCallback() const { return error_cb_; }
    void setReadCallback(const ReadCallback &read_cb);
    void setWriteCallback(const WriteCallback &write_cb);
    void setErrorCallback(const ErrorCallback &error_cb);

    virtual int read(char *buffer, size_t size);
    virtual int write(const char *buffer, size_t size);
    virtual bool setNonblock();
    virtual bool setCloseOnExec();

protected:
    IOService *io_service_;
    int64_t id_;
    DescriptorId fd_;
    ReadCallback read_cb_;
    WriteCallback write_cb_;
    ErrorCallback error_cb_;

private:
    BRICKRED_NONCOPYABLE(IODevice)
};

} // namespace brickred

#endif

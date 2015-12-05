#ifndef BRICKRED_DYNAMIC_BUFFER_H
#define BRICKRED_DYNAMIC_BUFFER_H

#include <cstddef>
#include <vector>

namespace brickred {

class DynamicBuffer {
public:
    DynamicBuffer(size_t init_size = 1024,
                  size_t expand_size = 1024);
    ~DynamicBuffer();
    void swap(DynamicBuffer &other);

    size_t capacity() const;
    size_t discardableBytes() const;
    size_t readableBytes() const;
    size_t writableBytes() const;

    const char *readBegin() const;
    char *writeBegin();
    void read(size_t size);
    void write(size_t size);
    void reserveWritableBytes(size_t size);
    void clear();

private:
    std::vector<char> buffer_;
    size_t expand_size_;
    size_t read_index_;
    size_t write_index_;
};

} // namespace brickred

#endif

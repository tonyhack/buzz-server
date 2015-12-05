#include <brickred/dynamic_buffer.h>

#include <cstring>
#include <algorithm>

namespace brickred {

DynamicBuffer::DynamicBuffer(size_t init_size, size_t expand_size) :
    buffer_(init_size), expand_size_(expand_size),
    read_index_(0), write_index_(0)
{
}

DynamicBuffer::~DynamicBuffer()
{
}

void DynamicBuffer::swap(DynamicBuffer &other)
{
    buffer_.swap(other.buffer_);
    std::swap(expand_size_, other.expand_size_);
    std::swap(read_index_, other.read_index_);
    std::swap(write_index_, other.write_index_);
}

size_t DynamicBuffer::capacity() const
{
    return buffer_.size();
}

size_t DynamicBuffer::discardableBytes() const
{
    return read_index_;
}

size_t DynamicBuffer::readableBytes() const
{
    return write_index_ - read_index_;
}

size_t DynamicBuffer::writableBytes() const
{
    return buffer_.size() - write_index_;
}

const char *DynamicBuffer::readBegin() const
{
    return &buffer_[read_index_];
}

char *DynamicBuffer::writeBegin()
{
    return &buffer_[write_index_];
}

void DynamicBuffer::read(size_t size)
{
    read_index_ += std::min(size, readableBytes());

    if (read_index_ == write_index_) {
        clear();
    }
}

void DynamicBuffer::write(size_t size)
{
    write_index_ += std::min(size, writableBytes());
}

void DynamicBuffer::reserveWritableBytes(size_t size)
{
    if (writableBytes() >= size) {
        return;
    }

    size_t free_bytes = discardableBytes() + writableBytes();
    size_t new_size = buffer_.size();

    if (free_bytes >= size && free_bytes - size < expand_size_) {
        new_size += expand_size_;
    } else {
        new_size += expand_size_ * ((size - free_bytes) / expand_size_ + 1);
    }

    size_t readable_bytes = readableBytes();
    std::vector<char> new_buffer(new_size);
    ::memcpy(&new_buffer[0], readBegin(), readable_bytes);

    buffer_.swap(new_buffer);
    read_index_ = 0;
    write_index_ = readable_bytes;
}

void DynamicBuffer::clear()
{
    read_index_ = 0;
    write_index_ = 0;
}

} // namespace brickred

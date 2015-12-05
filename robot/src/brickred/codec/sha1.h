#ifndef BRICKRED_CODEC_SHA1_H
#define BRICKRED_CODEC_SHA1_H

#include <stdint.h>
#include <cstddef>
#include <string>

#include <brickred/class_util.h>

namespace brickred {
namespace codec {

class Sha1 {
public:
    Sha1();
    ~Sha1();
    void reset();

    void update(const char *buffer, size_t size);
    void digest(char hash[20]);
    std::string digest();

private:
    BRICKRED_NONCOPYABLE(Sha1)

    uint32_t hash_[5];
    uint8_t work_block_[64];
    uint64_t message_size_;
};

std::string sha1(const std::string &str);
std::string sha1(const char *buffer, size_t size);
std::string sha1Binary(const std::string &str);
std::string sha1Binary(const char *buffer, size_t size);

} // namespace codec
} // namespace brickred

#endif

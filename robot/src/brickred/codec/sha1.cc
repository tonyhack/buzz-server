#include <brickred/codec/sha1.h>

#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <algorithm>

namespace brickred {
namespace codec {

Sha1::Sha1()
{
    reset();
}

Sha1::~Sha1()
{
}

void Sha1::reset()
{
    hash_[0] = 0x67452301;
    hash_[1] = 0xefcdab89;
    hash_[2] = 0x98badcfe;
    hash_[3] = 0x10325476;
    hash_[4] = 0xc3d2e1f0;
    message_size_ = 0;
}


static void sha1Processblock(uint32_t *hash, const uint8_t *work_block)
{
    #define ROTL32(_dword, _n) ((_dword) << (_n) ^ ((_dword) >> (32 - (_n))))

    uint32_t a = hash[0];
    uint32_t b = hash[1];
    uint32_t c = hash[2];
    uint32_t d = hash[3];
    uint32_t e = hash[4];
    uint32_t temp;
    uint32_t w[80];

    // init arary w
    for (int i = 0; i < 16; ++i) {
        w[i] = htonl(((uint32_t *)work_block)[i]);
    }
    for (int i = 16; i < 80; ++i) {
        w[i] = ROTL32(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    }
    // round 1
    for (int i = 0; i < 20; ++i) {
        temp = ROTL32(a, 5) + ((b & c) | ((~b) & d)) +
               e + w[i] + 0x5a827999;
        e = d; d = c; c = ROTL32(b, 30); b = a; a = temp;
    }
    // round 2
    for (int i = 20; i < 40; ++i) {
        temp = ROTL32(a, 5) + (b ^ c ^ d) +
               e + w[i] + 0x6ed9eba1;
        e = d; d = c; c = ROTL32(b, 30); b = a; a = temp;
    }
    // round 3
    for (int i = 40; i < 60; ++i) {
        temp = ROTL32(a, 5) + ((b & c) | (b & d) | (c & d)) +
               e + w[i] + 0x8f1bbcdc;
        e = d; d = c; c = ROTL32(b, 30); b = a; a = temp;
    }
    // round 4
    for (int i = 60; i < 80; ++i) {
        temp = ROTL32(a, 5) + (b ^ c ^ d) +
               e + w[i] + 0xca62c1d6;
        e = d; d = c; c = ROTL32(b, 30); b = a; a = temp;
    }

    hash[0] += a;
    hash[1] += b;
    hash[2] += c;
    hash[3] += d;
    hash[4] += e;

    #undef ROTL32
}

void Sha1::update(const char *buffer, size_t size)
{
    size_t wb_size = message_size_ & 63;
    message_size_ += size;

    // fill partial work block until it get full
    if (wb_size > 0) {
        size_t left = 64 - wb_size;
        ::memcpy(work_block_ + wb_size, buffer, std::min(size, left));

        // not full, wait for more data
        if (size < left) {
            return;
        }
        // process work block
        sha1Processblock(hash_, work_block_);
        buffer += left;
        size -= left;
    }

    // process left 64-byte block data without copy
    while (size >= 64) {
        sha1Processblock(hash_, (uint8_t *)buffer);
        buffer += 64;
        size -= 64;
    }

    // save leftover to work block
    if (size > 0) {
        ::memcpy(work_block_, buffer, size);
    }
}

void Sha1::digest(char hash[20])
{
    // pad with a 0x80, then 0x0, then length
    static const uint8_t pad[64] = {0x80};
    uint32_t pad_len[2];
    pad_len[0] = htonl(message_size_ >> 29);
    pad_len[1] = htonl(message_size_ << 3);

    size_t wb_size = message_size_ & 63;
    if (wb_size > 55) {
        // no room left to store 8-byte pad length + 1-byte 0x80
        // store pad length at next block
        update((const char *)pad, 64 - wb_size + 56);
        update((const char *)pad_len, 8);
    } else {
        update((const char *)pad, 56 - wb_size);
        update((const char *)pad_len, 8);
    }

    for (int i = 0; i < 5; ++i) {
        ((uint32_t *)hash)[i] = htonl(hash_[i]);
    }
}

std::string Sha1::digest()
{
    char hash[20];
    digest(hash);

    char hex_output[64];
    int count = 0;
    for (size_t i = 0; i < sizeof(hash); ++i) {
        count += ::snprintf(hex_output + count,
                            sizeof(hex_output) - count, "%02hhx", hash[i]);
    }
    return std::string(hex_output);
}

std::string sha1(const std::string &str)
{
    return sha1(str.c_str(), str.size());
}

std::string sha1(const char *buffer, size_t size)
{
    Sha1 ctx;

    ctx.update(buffer, size);

    return ctx.digest();
}

std::string sha1Binary(const std::string &str)
{
    return sha1Binary(str.c_str(), str.size());
}

std::string sha1Binary(const char *buffer, size_t size)
{
    Sha1 ctx;
    char hash[20];

    ctx.update(buffer, size);
    ctx.digest(hash);

    return std::string(hash, sizeof(hash));
}

} // namespace codec
} // namespace brickred

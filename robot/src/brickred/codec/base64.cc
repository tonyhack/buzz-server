#include <brickred/codec/base64.h>

#include <algorithm>
#include <vector>

namespace brickred {
namespace codec {

static const char s_encode_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                     "abcdefghijklmnopqrstuvwxyz"
                                     "0123456789+/";
static const char s_decode_table[] = {
    62, -1, -1, -1, 63, 52, 53, 54, 55, 56,
    57, 58, 59, 60, 61, -1, -1, -1, -2, -1,
    -1, -1,  0,  1,  2,  3,  4,  5,  6,  7,
     8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
    18, 19, 20, 21, 22, 23, 24, 25, -1, -1,
    -1, -1, -1, -1, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
    42, 43, 44, 45, 46, 47, 48, 49, 50, 51
};

int base64Encode(const char *in, size_t in_size,
                 char *out, size_t out_size)
{
    unsigned char b[3];
    const char *out_start = out;
    const char *out_end = out + out_size;

    for (; in_size > 0;) {
        int b_len = 0;

        if (in_size >= 3) {
            b[0] = *in++;
            b[1] = *in++;
            b[2] = *in++;
            b_len = 3;
        } else if (2 == in_size) {
            b[0] = *in++;
            b[1] = *in++;
            b[2] = 0;
            b_len = 2;
        } else if (1 == in_size) {
            b[0] = *in++;
            b[1] = 0;
            b[2] = 0;
            b_len = 1;
        }

        in_size -= b_len;

        // overflow
        if (out_end - out < 4) {
            return -1;
        }

        *out++ = s_encode_table[b[0] >> 2];
        *out++ = s_encode_table[((b[0] & 0x03) << 4) |
                                ((b[1] & 0xf0) >> 4)];
        *out++ = (b_len > 1)
               ? s_encode_table[((b[1] & 0x0f) << 2) |
                                ((b[2] & 0xc0) >> 6)]
               : '=';
        *out++ = (b_len > 2)
               ? s_encode_table[b[2] & 0x3f]
               : '=';
    }

    return out - out_start;
}

std::string base64Encode(const std::string &str)
{
    return base64Encode(str.c_str(), str.size());
}

std::string base64Encode(const char *buffer, size_t size)
{
    std::vector<char> output(std::max((size_t)5, size * 2));
    int count = base64Encode(buffer, size, &output[0], output.size());
    if (count <= 0) {
        return std::string();
    } else {
        return std::string(&output[0], count);
    }
}

int base64Decode(const char *in, size_t in_size,
                 char *out, size_t out_size)
{
    unsigned char b[4];
    const char *out_start = out;
    const char *out_end = out + out_size;

    for (; in_size > 0;) {
        if (in_size < 4) {
            return -1;
        }

        int b_len = 0;

        for (int i = 0; i < 4; ++i) {
            char c = *in++;
            if (c < 43 || c > 122 || '=' == c) {
                b[i] = 0;
            } else {
                b[i] = s_decode_table[c - 43];
                ++b_len;
            }
        }

        if (b_len < 2) {
            return -1;
        }

        // overflow
        if (out_end - out < b_len - 1) {
            return -1;
        }

        *out++ = b[0] << 2 | b[1] >> 4;
        if (b_len > 2) {
            *out++ = b[1] << 4 | b[2] >> 2;
        }
        if (b_len > 3) {
            *out++ = b[2] << 6 | b[3];
        }

        in_size -= 4;
    }

    return out - out_start;
}

std::string base64Decode(const std::string &str)
{
    return base64Decode(str.c_str(), str.size());
}

std::string base64Decode(const char *buffer, size_t size)
{
    std::vector<char> output(size);
    int count = base64Decode(buffer, size, &output[0], output.size());
    if (count <= 0) {
        return std::string();
    } else {
        return std::string(&output[0], count);
    }
}

} // namespace codec
} // namespace brickred

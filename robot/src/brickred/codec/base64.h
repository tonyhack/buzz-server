#ifndef BRICKRED_CODEC_BASE64_H
#define BRICKRED_CODEC_BASE64_H

#include <cstddef>
#include <string>

namespace brickred {
namespace codec {

int base64Encode(const char *in, size_t in_size,
                  char *out, size_t out_size);
std::string base64Encode(const std::string &str);
std::string base64Encode(const char *buffer, size_t size);

int base64Decode(const char *in, size_t in_size,
                  char *out, size_t out_size);
std::string base64Decode(const std::string &str);
std::string base64Decode(const char *buffer, size_t size);

} // namespace codec
} // namespace brickred

#endif

#ifndef BUZZ_ROBOT_THRIFT_CODEC_H
#define BUZZ_ROBOT_THRIFT_CODEC_H

#include <stdint.h>
#include <cstddef>
#include <brickred/function.h>
#include <thrift/protocol/TProtocol.h>

namespace robot {
namespace thrift_codec {

typedef brickred::Function<uint32_t (apache::thrift::protocol::TProtocol *)>
        ThriftCodecFunc;

int encode(ThriftCodecFunc codec_func, char *buffer, size_t size);
int decode(ThriftCodecFunc codec_func, const char *buffer, size_t size);

template <class T>
int encode(const T &obj, char *buffer, size_t size)
{
    ThriftCodecFunc codec_func =
        BRICKRED_BIND_TEMPLATE_MEM_FUNC(&T::write, &obj);
    return encode(codec_func, buffer, size);
}

template <class T>
int decode(T &obj, const char *buffer, size_t size) 
{
    ThriftCodecFunc codec_func =
        BRICKRED_BIND_TEMPLATE_MEM_FUNC(&T::read, &obj);
    return decode(codec_func, buffer, size);
}

} // namespace thrift_codec
} // namespace robot

#endif


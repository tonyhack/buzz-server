#include "robot/thrift_codec.h"

#include <cstring>
#include <boost/shared_ptr.hpp>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>

namespace robot {
namespace thrift_codec {

using apache::thrift::transport::TMemoryBuffer;
using apache::thrift::protocol::TBinaryProtocol;
using apache::thrift::TException;

int encode(ThriftCodecFunc codec_func, char *buffer, size_t size)
{
    try {
        boost::shared_ptr<TMemoryBuffer> thrift_buffer(new TMemoryBuffer());
        TBinaryProtocol thrift_protocol(thrift_buffer);

        if (codec_func(&thrift_protocol) <= 0) {
            return -1;
        }

        uint8_t *inner_buffer = NULL;
        uint32_t inner_buffer_size = 0;

        thrift_buffer->getBuffer(&inner_buffer, &inner_buffer_size);

        if (inner_buffer_size > size) {
            return -1;
        }
        ::memcpy(buffer, inner_buffer, inner_buffer_size);
        return inner_buffer_size;

    } catch (const TException &ex) {
        return -1;
    }
}

int decode(ThriftCodecFunc codec_func, const char *buffer, size_t size)
{
    try {
        boost::shared_ptr<TMemoryBuffer> thrift_buffer(
            new TMemoryBuffer((uint8_t *)buffer, (uint32_t)size));
        TBinaryProtocol thrift_protocol(thrift_buffer);

        uint32_t ret = codec_func(&thrift_protocol);
        return ret;

    } catch (const TException &ex) {
        return -1;
    }
}

} // namespace thrift_codec
} // namespace robot


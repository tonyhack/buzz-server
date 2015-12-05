#include <brickred/protocol/http_protocol.h>

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <string>
#include <vector>

#include <brickred/dynamic_buffer.h>
#include <brickred/string_util.h>
#include <brickred/protocol/http_message.h>
#include <brickred/protocol/http_request.h>
#include <brickred/protocol/http_response.h>

namespace brickred {
namespace protocol {

class HttpProtocol::Impl {
public:
    typedef HttpProtocol::Status Status;
    typedef HttpProtocol::RetCode RetCode;
    typedef int (HttpProtocol::Impl::*StatusHandler)(DynamicBuffer *);

    Impl();
    ~Impl();
    void reset();

    Status::type getStatus() const { return status_; }
    RetCode::type recvMessage(DynamicBuffer *buffer);
    bool retrieveRequest(HttpRequest *request);
    bool retrieveResponse(HttpResponse *response);

public:
    int readStartLine(DynamicBuffer *buffer);
    int readHeader(DynamicBuffer *buffer);
    int readBody(DynamicBuffer *buffer);

private:
    static StatusHandler s_status_handler_[Status::MAX];

private:
    Status::type status_;
    HttpMessage *message_;
    DynamicBuffer *chunk_buffer_;
};

///////////////////////////////////////////////////////////////////////////////
HttpProtocol::Impl::StatusHandler HttpProtocol::Impl::s_status_handler_[] = {
    &HttpProtocol::Impl::readStartLine,
    &HttpProtocol::Impl::readHeader,
    &HttpProtocol::Impl::readBody,
    NULL,
    NULL
};

HttpProtocol::Impl::Impl() :
    status_(Status::READING_START_LINE), message_(NULL), chunk_buffer_(NULL)
{
}

HttpProtocol::Impl::~Impl()
{
    reset();
}

void HttpProtocol::Impl::reset()
{
    if (message_ != NULL) {
        delete message_;
        message_ = NULL;
    }
    
    if (chunk_buffer_ != NULL) {
        delete chunk_buffer_;
        chunk_buffer_ = NULL;
    }

    status_ = Status::READING_START_LINE;
}

HttpProtocol::Impl::RetCode::type HttpProtocol::Impl::recvMessage(
    DynamicBuffer *buffer)
{
    for (;;) {
        StatusHandler func = s_status_handler_[status_];
        if (NULL == func) {
            return RetCode::ERROR;
        }

        int ret = (this->*func)(buffer);
        if (0 == ret) {
            // not finished, wait for more data
            return RetCode::WAITING_MORE_DATA;

        } else if (-1 == ret) {
            // error occured
            status_ = Status::PENDING_ERROR;
            return RetCode::ERROR;
        }

        if (Status::FINISHED == status_) {
            // finished
            return RetCode::MESSAGE_READY;
        }
    }
}

int HttpProtocol::Impl::readStartLine(DynamicBuffer *buffer)
{
    // get a http line
    const char *crlf = string_util::find(buffer->readBegin(),
        buffer->readableBytes(), "\r\n");
    if (NULL == crlf) {
        return 0;
    }

    size_t line_length = crlf - buffer->readBegin();

    // split line to three parts
    std::vector<std::string> start_line_parts;
    string_util::split(buffer->readBegin(), line_length,
                       " ", &start_line_parts, 3);
    if (start_line_parts.size() != 3) {
        return -1;
    }

    if (::memcmp(start_line_parts[0].c_str(), "HTTP", 4) == 0) {
        // response version
        HttpMessage::Version::type version =
            HttpMessage::VersionStrToEnum(start_line_parts[0]);
        if (HttpMessage::Version::UNKNOWN == version) {
            return -1;
        }

        // response status code
        int status_code = ::atoi(start_line_parts[1].c_str());
        if (status_code < 0) {
            return -1;
        }

        // build http response
        UniquePtr<HttpResponse> response(new HttpResponse());
        response->setVersion(version);
        response->setStatusCode(status_code);
        response->setReasonPhrase(start_line_parts[2]);

        buffer->read(line_length + 2);
        this->message_ = response.release();
        this->status_ = Status::READING_HEADER;

        return 1;

    } else {
        // request method
        HttpRequest::Method::type method =
            HttpRequest::MethodStrToEnum(start_line_parts[0]);
        if (HttpRequest::Method::UNKNOWN == method) {
            return -1;
        }

        // request version
        HttpMessage::Version::type version =
            HttpMessage::VersionStrToEnum(start_line_parts[2]);
        if (HttpMessage::Version::UNKNOWN == version) {
            return -1;
        }

        // build http request
        UniquePtr<HttpRequest> request(new HttpRequest());
        request->setMethod(method);
        request->setRequestUri(start_line_parts[1]);
        request->setVersion(version);

        buffer->read(line_length + 2);
        this->message_ = request.release();
        this->status_ = Status::READING_HEADER;

        return 1;
    }
}

int HttpProtocol::Impl::readHeader(DynamicBuffer *buffer)
{
    if (buffer->readableBytes() < 2) {
        return 0;
    }

    if (::memcmp(buffer->readBegin(), "\r\n", 2) == 0) {
        buffer->read(2);
        this->status_ = Status::FINISHED;
        return 1;
    }

    const char *double_crlf = string_util::find(buffer->readBegin(),
        buffer->readableBytes(), "\r\n\r\n");
    if (NULL == double_crlf) {
        return 0;
    }

    size_t header_length = double_crlf - buffer->readBegin();

    // split headers with \r\n
    std::vector<std::string> headers;
    string_util::split(buffer->readBegin(), header_length, "\r\n", &headers);

    for (size_t i = 0; i < headers.size(); ++i) {
        const char *colon = string_util::find(
            headers[i].c_str(), headers[i].size(), ":");
        if (NULL == colon) {
            return -1;
        }

        message_->setHeader(
            string_util::trim(std::string(headers[i].c_str(), colon)),
            string_util::trim(std::string(colon + 1)));
    }

    buffer->read(header_length + 4);
    this->status_ = Status::READING_BODY;
    return 1;
}

int HttpProtocol::Impl::readBody(DynamicBuffer *buffer)
{
    // header content-length exists
    int content_length = ::atoi(message_->getHeader("Content-Length").c_str());
    if (content_length > 0) {
        if (buffer->readableBytes() < (size_t)content_length) {
            // wait for more data
            return 0;
        }

        message_->setBody(buffer->readBegin(), content_length);

        buffer->read(content_length);
        this->status_ = Status::FINISHED;
        return 1;
    }

    // header transfer-encoding == "chunked"
    if (message_->headerContain("Transfer-Encoding", "chunked")) {
        if (NULL == chunk_buffer_) {
            chunk_buffer_ = new DynamicBuffer();
        }

        for (;;) {
            const char *buffer_start = buffer->readBegin();
            size_t buffer_size = buffer->readableBytes();

            const char *crlf =
                string_util::find(buffer_start, buffer_size, "\r\n");
            if (NULL == crlf) {
                // wait for more data
                return 0;
            }

            std::string chunk_head_line(buffer_start, crlf);
            unsigned int chunk_size = 0;
            if (::sscanf(chunk_head_line.c_str(), "%x", &chunk_size) != 1) {
                return -1;
            }

            size_t total_chunk_size = (0 == chunk_size)
                    ? chunk_head_line.size() + 2
                    : chunk_head_line.size() + 2 + chunk_size + 2;

            if (buffer_size < total_chunk_size) {
                // wait for more data
                return 0;
            }
            if (::memcmp(crlf + 2 + chunk_size, "\r\n", 2) != 0) {
                return -1;
            }

            if (chunk_size > 0) {
                chunk_buffer_->reserveWritableBytes(chunk_size);
                ::memcpy(chunk_buffer_->writeBegin(), crlf + 2, chunk_size);
                chunk_buffer_->write(chunk_size);
            }
            buffer->read(total_chunk_size);

            if (0 == chunk_size) {
                // set body
                message_->setBody(chunk_buffer_->readBegin(),
                                  chunk_buffer_->readableBytes());
                // set content-length header
                message_->setHeader("Content-Length",
                    string_util::toString(chunk_buffer_->readableBytes()));
                // remove transfer-encoding header
                message_->removeHeader("Transfer-Encoding");

                // read tailer header
                this->status_ = Status::READING_HEADER;
                return 1;
            }
        }

    }

    this->status_ = Status::FINISHED;
    return 1;
}

bool HttpProtocol::Impl::retrieveRequest(HttpRequest *request)
{
    if (status_ != Status::FINISHED) {
        return false;
    }
    if (NULL == message_) {
        return false;
    }
    if (message_->getMessageType() != HttpMessage::MessageType::REQUEST) {
        return false;
    }

    request->swap(*static_cast<HttpRequest *>(this->message_));
    reset();

    return true;
}

bool HttpProtocol::Impl::retrieveResponse(HttpResponse *response)
{
    if (status_ != Status::FINISHED) {
        return false;
    }
    if (NULL == message_) {
        return false;
    }
    if (message_->getMessageType() != HttpMessage::MessageType::RESPONSE) {
        return false;
    }

    response->swap(*static_cast<HttpResponse *>(this->message_));
    reset();

    return true;
}

///////////////////////////////////////////////////////////////////////////////
HttpProtocol::HttpProtocol() :
    pimpl_(new Impl())
{
}

HttpProtocol::~HttpProtocol()
{
}

void HttpProtocol::reset()
{
    pimpl_->reset();
}

HttpProtocol::Status::type HttpProtocol::getStatus() const
{
    return pimpl_->getStatus();
}

HttpProtocol::RetCode::type HttpProtocol::recvMessage(DynamicBuffer *buffer)
{
    return pimpl_->recvMessage(buffer);
}

bool HttpProtocol::retrieveRequest(HttpRequest *request)
{
    return pimpl_->retrieveRequest(request);
}

bool HttpProtocol::retrieveResponse(HttpResponse *response)
{
    return pimpl_->retrieveResponse(response);
}

void HttpProtocol::writeMessage(const HttpMessage &message,
                                DynamicBuffer *buffer)
{
    int count = 0;

    // start line
    if (message.getMessageType() == HttpMessage::MessageType::REQUEST) {
        const HttpRequest &request =
            static_cast<const HttpRequest &>(message);

        buffer->reserveWritableBytes(64 + request.getRequestUri().size());
        count = ::snprintf(buffer->writeBegin(), buffer->writableBytes(),
            "%s %s %s\r\n",
            HttpRequest::MethodEnumToStr(request.getMethod()).c_str(),
            request.getRequestUri().c_str(),
            HttpMessage::VersionEnumToStr(request.getVersion()).c_str());
        buffer->write(count);

    } else if (message.getMessageType() ==
               HttpMessage::MessageType::RESPONSE) {
        const HttpResponse &response =
            static_cast<const HttpResponse &>(message);

        buffer->reserveWritableBytes(64 + response.getReasonPhrase().size());
        count = ::snprintf(buffer->writeBegin(), buffer->writableBytes(),
            "%s %d %s\r\n",
            HttpMessage::VersionEnumToStr(response.getVersion()).c_str(),
            response.getStatusCode(),
            response.getReasonPhrase().c_str());
        buffer->write(count);

    } else {
        return;
    }

    // header
    for (HttpMessage::HeaderMap::const_iterator iter =
             message.getHeaders().begin();
         iter != message.getHeaders().end(); ++iter) {
        buffer->reserveWritableBytes(
            32 + iter->first.size() + iter->second.size());
        count = ::snprintf(buffer->writeBegin(), buffer->writableBytes(),
            "%s: %s\r\n", iter->first.c_str(), iter->second.c_str());
        buffer->write(count);
    }
    buffer->reserveWritableBytes(2);
    ::snprintf(buffer->writeBegin(), buffer->writableBytes(), "\r\n");
    buffer->write(2);

    // body
    buffer->reserveWritableBytes(message.getBody().size());
    count = ::snprintf(buffer->writeBegin(), buffer->writableBytes(), "%s",
                       message.getBody().c_str());
    buffer->write(count);
}


} // namespace protocol
} // namespace brickred

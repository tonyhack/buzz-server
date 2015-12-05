//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-05 11:56:36.
// File name: http_service_handler.h
//
// Description:
// Define class HttpServiceHandler.
//

#ifndef __LOGIN__SERVER__HTTP__SERVICE__HANDLER__H
#define __LOGIN__SERVER__HTTP__SERVICE__HANDLER__H

#include <boost/function.hpp>

#include "core/base/byte_buffer_dynamic.h"
#include "core/base/noncopyable.h"
#include "core/base/time_tick.h"

namespace core {

class TCPConnection;
typedef boost::shared_ptr<TCPConnection> TCPConnectionPtr;

}  // namespace core

namespace login {

namespace server {

class HttpContext;
class HttpRequest;

class HttpServiceHandler : public core::Noncopyable {
  typedef boost::function<bool (const core::TCPConnectionPtr &)> HttpCallback;
 public:
  HttpServiceHandler();
  ~HttpServiceHandler();

  inline void SetHttpCallback(const HttpCallback &callback) {
    this->http_callback_ = callback;
  }

  void OnConnection(const core::TCPConnectionPtr &connection);
  void OnMessage(const core::TCPConnectionPtr &connection,
      core::ByteBufferDynamic &buffer, const core::TimeTick &tick);
  void OnWriteComplete(const core::TCPConnectionPtr &connection);

 private:
  HttpCallback http_callback_;

  bool ProcessRequestLine(const char* begin, const char* end,
      HttpContext* context);

  bool ParseRequest(core::ByteBufferDynamic *buf, HttpContext* context,
      const core::TimestampMillisecond& timestamp);

  static const char kCRLF[];
  static const char kBadRequest[];
};

}  // namespace server

}  // namespace login

#endif  // __LOGIN__SERVER__HTTP__SERVICE__HANDLER__H


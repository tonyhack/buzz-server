//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-05 12:15:25.
// File name: http_context.h
//
// Description:
// Define class HttpContext.
//

#ifndef __LOGIN__SERVER__HTTP__CONTEXT__H
#define __LOGIN__SERVER__HTTP__CONTEXT__H

#include "login_server_informal/server/http_request.h"

namespace login {

namespace server {

class HttpContext {
 public:
  enum HttpRequestParseState {
    EXPECT_REQUEST_LINE = 0,
    EXPECT_HEADER,
    EXPECT_BODY,
    EXPECT_COMPLETE,
  };

  HttpContext() : state_(EXPECT_REQUEST_LINE) {}

  inline bool CheckExpectRequestLine() const {
    return this->state_ == EXPECT_REQUEST_LINE;
  }
  inline bool CheckExpectHeader() const {
    return this->state_ == EXPECT_HEADER;
  }
  inline bool CheckExpectBody() const {
    return this->state_ == EXPECT_BODY;
  }
  inline bool CheckExpectAll() const {
    return this->state_ >= EXPECT_COMPLETE;
  }

  inline void ReceiveRequestLine() {
    this->state_ = EXPECT_HEADER;
  }
  inline void ReceiveHeader() {
    this->state_ = EXPECT_BODY;
  }
  inline void ReceiveBody() {
    this->state_ = EXPECT_COMPLETE;
  }
  inline void ReceiveAll() {
    this->state_ = EXPECT_COMPLETE;
  }

  void Reset() {
    this->state_ = EXPECT_REQUEST_LINE;
    request_.Reset();
  }

  const HttpRequest &GetRequest() const { return this->request_; }
  HttpRequest &GetRequest() { return this->request_; }

 private:
  HttpRequestParseState state_;
  HttpRequest request_;
};

}  // namespace server

}  // namespace login

#endif  // __LOGIN__SERVER__HTTP__CONTEXT__H


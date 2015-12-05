//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-05 13:44:39.
// File name: http_service_handler.cc
//
// Description:
// Define class HttpServiceHandler.
//

#include "login_server_informal/server/http_service_handler.h"

#include <algorithm>
#include <boost/any.hpp>
#include <boost/bind.hpp>

#include "core/tcp_connection.h"
#include "global/logging.h"
#include "login_server_informal/server/http_context.h"
#include "login_server_informal/server/http_request.h"

namespace login {

namespace server {

const char HttpServiceHandler::kCRLF[] = "\r\n";
const char HttpServiceHandler::kBadRequest[] = "HTTP/1.1 400 Bad Request\r\n\r\n";

HttpServiceHandler::HttpServiceHandler() {}
HttpServiceHandler::~HttpServiceHandler() {}

void HttpServiceHandler::OnConnection(const core::TCPConnectionPtr &connection) {
  if(connection->CheckConnected()) {
    connection->GetContext() = HttpContext();
    global::LogInfo("Connection establish.");
  } else {
    HttpContext *context = &boost::any_cast<HttpContext &>(connection->GetContext());
    assert(context);
    context->Reset();
    global::LogInfo("Connection disconnected.");
  }
}

void HttpServiceHandler::OnMessage(const core::TCPConnectionPtr &connection,
    core::ByteBufferDynamic &buffer, const core::TimeTick &time_tick) {
  HttpContext *context = &boost::any_cast<HttpContext &>(connection->GetContext()); 
  assert(context);
  core::TimestampMillisecond timestamp(time_tick);
  if(this->ParseRequest(&buffer, context, timestamp) == false) {
    connection->SendMessage(kBadRequest);
    connection->Shutdown();
  } else {
    if(context->CheckExpectAll()) {
      if(this->http_callback_(connection) == false) {
        context->GetRequest().SetCloseConnection();
        connection->SendMessage(kBadRequest);
        connection->Shutdown();
      }
    }
  }
}

void HttpServiceHandler::OnWriteComplete(const core::TCPConnectionPtr &connection){
  global::LogInfo("%s:%d (%s) Connection response complete.",
      __FILE__, __LINE__, __FUNCTION__);
  assert(connection);
  HttpContext *context = &boost::any_cast<HttpContext &>(connection->GetContext());
  if(context) {
    std::string check_connection(context->GetRequest().GetHeader("Connection"));
    bool close = check_connection == "close" ||
      context->GetRequest().GetVersion() == HttpRequest::HTTP10 ||
      check_connection != "Keep-Alive";
    if(close || context->GetRequest().CheckCloseConnection()) {
      connection->Shutdown();
    }
  } else {
    connection->Shutdown();
  }
  context->Reset();
}

bool HttpServiceHandler::ProcessRequestLine(const char* begin, const char* end,
    HttpContext* context) {
  bool succeed = false;
  const char* start = begin;
  const char* space = std::find(start, end, ' ');
  HttpRequest& request = context->GetRequest();
  if(space != end && request.SetMethod(start, space)) {
    start = space+1;
    space = std::find(start, end, ' ');
    if(space != end) {
      request.SetPath(start, space);
      start = space+1;
      succeed = end-start == 8 && std::equal(start, end-1, "HTTP/1.");
      if(succeed) {
        if(*(end-1) == '1') {
          request.SetVersion(HttpRequest::HTTP11);
        } else if (*(end-1) == '0') {
          request.SetVersion(HttpRequest::HTTP10);
        } else {
          succeed = false;
        }
      }
    }
  }
  return succeed;
}

bool HttpServiceHandler::ParseRequest(ByteBufferDynamic *buf, HttpContext* context,
    const core::TimestampMillisecond& timestamp) {
  bool ok = true;
  bool hasMore = true;
  while(hasMore) {
    if(context->CheckExpectRequestLine()) {
      if(buf->ReadSize() >= 2) {
        const char* crlf = std::search(buf->ReadPointer(),
            static_cast<const char*>(buf->WritePointer()), kCRLF, kCRLF + 2);
        if(crlf < buf->WritePointer()) {
          ok = ProcessRequestLine(buf->ReadPointer(), crlf, context);
          if(ok) {
            context->GetRequest().SetReceiveTime(timestamp);
            assert(buf->ReadPointer() <= crlf + 2);
            buf->ReadShift(crlf + 2 - buf->ReadPointer());
            context->ReceiveRequestLine();
          } else {
            hasMore = false;
          }
        } else {
          hasMore = false;
        }
      } else {
        hasMore = false;
      }
    } else if(context->CheckExpectHeader()) {
      if(buf->ReadSize() >= 2) {
        const char* crlf = std::search(buf->ReadPointer(),
            static_cast<const char*>(buf->WritePointer()), kCRLF, kCRLF + 2);
        if(crlf < buf->WritePointer()) {
          const char* colon = std::find(buf->ReadPointer(), crlf, ':');
          if(colon != crlf) {
            context->GetRequest().AddHeader(buf->ReadPointer(), colon, crlf);
          } else {
            context->ReceiveHeader();
          }
          assert(buf->ReadPointer() <= crlf + 2);
          buf->ReadShift(crlf + 2 - buf->ReadPointer());
        } else {
          hasMore = false;
        }
      } else {
        hasMore = false;
      }
    } else if(context->CheckExpectBody()) {
      HttpRequest& request = context->GetRequest();
      if(request.GetMethod() == HttpRequest::POST) {
        int content_length = request.GetContentLength();
        if(content_length > 0) {
          if(content_length <= buf->ReadSize()) {
            global::LogDebug("content_length[%d] buf->ReadSize[%d].",
                content_length, buf->ReadSize());
            request.SetContent(buf->ReadPointer(), content_length);
            buf->ReadShift(buf->ReadSize());
            context->ReceiveBody();
          } else {
            global::LogDebug("content_length[%d] buf->ReadSize[%d].",
                content_length, buf->ReadSize());
          }
          hasMore = false;
        } else {
          global::LogDebug("content_length[%d].", content_length);
          context->ReceiveAll();
          hasMore = false;
        }
      } else {
        context->ReceiveAll();
        hasMore = false;
      }
    } else if(context->CheckExpectAll()) {
      hasMore = false;
    }
  }
  return ok;
}

}  // namespace server

}  // namespace login


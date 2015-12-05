//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-05 13:35:57.
// File name: http_response.h
//
// Description:
// Define class HttpResponse.
//

#ifndef __LOGIN__SERVER__HTTP__RESPONSE__H
#define __LOGIN__SERVER__HTTP__RESPONSE__H

#include <stdio.h>
#include <string.h>

#include <map>
#include <string>

#include "core/base/byte_buffer_dynamic.h"
#include "core/base/noncopyable.h"

namespace login {

namespace server {

class HttpResponse : public core::Noncopyable {
 public:
  enum HttpStatusCode {
    STATUS_UNKNOWN = 0,
    STATUS_200_OK = 200,
    STATUS_301_MOVE_PERMANENTLY = 301,
    STATUS_400_BAD_REQUEST = 400,
    STATUS_404_NOT_FOUND = 404,
  };

  HttpResponse() : status_(STATUS_UNKNOWN), close_connection_(false) {}

  explicit HttpResponse(bool close)
         : status_(STATUS_UNKNOWN), close_connection_(close) {}

  ~HttpResponse() {}

  inline void SetStatusCode(HttpStatusCode status) {
    this->status_ = status;
  }
  inline void SetStatusMessage(const std::string &message) {
    this->status_message_ = message;
  }
  inline void SetCloseConnection(bool on) {
    this->close_connection_ = on;
  }
  inline bool CheckCloseConnection() const { return this->close_connection_; }
  inline void SetContentType(const std::string& content_type) {
    AddHeader("Content-Type", content_type);
  }
  inline void AddHeader(const std::string &key, const std::string &value) {
    this->header_[key] = value;
  }
  inline void SetBody(const std::string &body) {
    this->body_ = body;
  }
  inline void SetBody(const char *buffer, size_t size) {
    this->body_.assign(buffer, size);
  }

  void ToBuffer(core::ByteBufferDynamic *buffer) {
    char buf[32];
    snprintf(buf, sizeof(buf), "HTTP/1.1 %d ", this->status_);
    size_t len = strlen(buf);
    buffer->PreWrite(len);
    memcpy(buffer->WritePointer(), buf, len);
    buffer->WriteShift(len);

    len = this->status_message_.length();
    buffer->PreWrite(len);
    memcpy(buffer->WritePointer(), this->status_message_.c_str(), len);
    buffer->WriteShift(len);

    buffer->PreWrite(krn_size);
    memcpy(buffer->WritePointer(), krn_str, krn_size);
    buffer->WriteShift(krn_size);

    if(this->close_connection_) {
      buffer->PreWrite(kclose_connection_size);
      memcpy(buffer->WritePointer(), kclose_connection_str,
          kclose_connection_size);
          buffer->WriteShift(kclose_connection_size);
    } else {
      snprintf(buf, sizeof(buf), "Content-Length: %zd\r\n", this->body_.size());
      len = strlen(buf);
      buffer->PreWrite(len + kkeep_alive_size);
      memcpy(buffer->WritePointer(), buf, len);
      buffer->WriteShift(len);
      memcpy(buffer->WritePointer(), kkeep_alive_str, kkeep_alive_size);
      buffer->WriteShift(kkeep_alive_size);
    }

    for(std::map<std::string, std::string>::const_iterator iter = this->header_.begin();
        iter != this->header_.end(); ++iter) {
      len = iter->first.length();
      buffer->PreWrite(len);
      memcpy(buffer->WritePointer(), iter->first.c_str(), iter->first.length());
      buffer->WriteShift(len);

      len = 2;
      buffer->PreWrite(len);
      memcpy(buffer->WritePointer(), ": ", 2);
      buffer->WriteShift(len);

      len = iter->second.length();
      buffer->PreWrite(len);
      memcpy(buffer->WritePointer(), iter->second.c_str(), iter->second.length());
      buffer->WriteShift(len);

      buffer->PreWrite(krn_size);
      memcpy(buffer->WritePointer(), krn_str, krn_size);
      buffer->WriteShift(krn_size);
    }

    buffer->PreWrite(krn_size);
    memcpy(buffer->WritePointer(), krn_str, krn_size);
    buffer->WriteShift(krn_size);

    len = this->body_.size();
    buffer->PreWrite(len);
    memcpy(buffer->WritePointer(), this->body_.data(), len);
    buffer->WriteShift(len);
  }

 private:
  static const char *krn_str;
  static const size_t krn_size;
  static const char *kclose_connection_str;
  static const size_t kclose_connection_size;
  static const char *kkeep_alive_str;
  static const size_t kkeep_alive_size;

  std::map<std::string, std::string> header_;
  HttpStatusCode status_;
  std::string status_message_;
  bool close_connection_;
  std::string body_;
};

}  // namespace server

}  // namespace login

#endif  // __LOGIN__SERVER__HTTP__RESPONSE__H


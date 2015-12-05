//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-05 12:23:40.
// File name: http_request.h
//
// Description:
// Define class HttpRequest.
//

#ifndef __LOGIN__SERVER__HTTP__REQUEST__H
#define __LOGIN__SERVER__HTTP__REQUEST__H

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>

#include <map>
#include <string>

#include "core/base/time_tick.h"

using namespace core;

namespace login {

namespace server {

class HttpRequest {
 public:
  enum Method { INVALID = 0, GET, POST, HEAD, PUT, DELETE, };
  enum Version { UNKNOWN = 0, HTTP10, HTTP11, };

  HttpRequest() : method_(INVALID), version_(UNKNOWN), close_connection_(false), content_length_(0) {}
  ~HttpRequest() {}

  inline void SetVersion(Version version) { this->version_ = version; }
  inline Version GetVersion() const { return this->version_; }

  bool SetMethod(const char *start, const char *end) {
    assert(this->method_ == INVALID);
    std::string method(start, end);
    if(method == "GET") {
      this->method_ = GET;
    } else if(method == "POST") {
      this->method_ = POST;
    } else {
      this->method_ = INVALID;
    }
    return this->method_ != INVALID;
  }
  inline Method GetMethod() const { return this->method_; }
  inline const char *GetMethodString() const {
    const char *result = "UNKNOWN";
    switch(this->method_) {
      case GET:
        result = "GET";
        break;
      case POST:
        result = "POST";
        break;
      default:
        break;
    }
    return result;
  }

  inline void SetPath(const char *start, const char *end) {
    this->path_.assign(start, end);
  }
  inline const std::string &GetPath() const { return this->path_; }

  inline void SetReceiveTime(const core::TimestampMillisecond &timestamp) { this->receive_time_ = timestamp; }
  inline const core::TimestampMillisecond &GetReceiveTime() const { return this->receive_time_; }

  inline void SetCloseConnection() { this->close_connection_ = true; }
  inline bool CheckCloseConnection() const { return this->close_connection_; }

  void AddHeader(const char *start, const char *colon, const char *end) {
    std::string field(start, colon);
    std::transform(field.begin(), field.end(), field.begin(), ::tolower);
    ++colon;
    while (isspace(*colon)) { 
      ++colon;
    } 
    std::string value(colon, end);
    while (!value.empty() && isspace(value[value.size()-1])) {   
      value.resize(value.size()-1);
    }
    header_[field] = value;
    if(this->GetMethod() == POST && field == "content-length") {
      this->content_length_ = atoi(value.c_str());
    }
  }

  std::string GetHeader(const std::string& field) const {     
    std::string result;
    std::map<std::string, std::string>::const_iterator it = this->header_.find(field);
    if (it != this->header_.end()) { 
      result = it->second;
    } 
    return result;
  }

  const std::map<std::string, std::string>& GetHeader() const { return this->header_; }

  inline int GetContentLength() const { return this->content_length_; }

  inline void SetContent(const char *start, size_t size) {
    this->content_length_ = size;
    this->content_.assign(start, this->content_length_);
  }
  inline const std::string &GetContent() const { return this->content_; }

  void Swap(HttpRequest& that) {
    std::swap(this->method_, that.method_);
    this->path_.swap(that.path_);
    this->receive_time_ = that.receive_time_;
    this->header_.swap(that.header_);
    this->content_.swap(that.content_);
    this->content_length_ = that.content_length_;
  }

  void Reset() {
    this->method_ = INVALID;
    this->version_ = UNKNOWN;
    this->header_.clear();
    this->content_.clear();
    this->content_length_ = 0;
  }

 private:
  Method method_;
  Version version_;
  std::string path_;
  core::TimestampMillisecond receive_time_;
  bool close_connection_;
  std::map<std::string, std::string> header_;
  int content_length_;
  std::string content_;
};

}  // namespace server

}  // namespace login

#endif  // __LOGIN__SERVER__HTTP__REQUEST__H


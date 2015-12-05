//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-02 14:27:21.
// File name: request_proxy.h
//
// Description:
// Define class RequestProxy.
//

#ifndef __GAME__SERVER__PYTHON__REQUEST__PROXY__H
#define __GAME__SERVER__PYTHON__REQUEST__PROXY__H

#include <boost/shared_ptr.hpp>

#include "game_server/server/request/game_request_types.h"
#include "game_server/server/extension_manager.h"
#include "global/global_packet.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace python {

class RequestProxy {
 public:
  RequestProxy();
  ~RequestProxy();

  bool Initialize();
  void Finalize();

  // int Request(request::RequestType::type type, const char *message, size_t size);
  int Request(int type, const char *message, size_t size);

  const std::string GetResult() const {
    return this->result_;
  }

 private:
  template <typename T>
  //int RequestHandler(request::RequestType::type type, const char *message, size_t size);
  int RequestHandler(int type, const char *message, size_t size);

  std::string result_;
};

template <typename T>
//int RequestProxy::RequestHandler(request::RequestType::type type, const char *message, size_t size) {
int RequestProxy::RequestHandler(int type, const char *message, size_t size) {
  T request;
  if(global::GlobalPacket::GetPacket()->Deserialize(request, message, size) == false) {
    global::LogError("%s:%d (%s) 消息反序列化失败 size=%lu, type=%d",
        __FILE__, __LINE__, __FUNCTION__, size, type);
    return -1;
  }
  if(ExtensionManager::GetInstance()->Request((request::RequestType::type)type,
        &request, sizeof(request)) != 0) {
    global::LogError("%s:%d (%s) 消息请求失败",
        __FILE__, __LINE__, __FUNCTION__);
    return -1;
  }
  // 序列化结果
  const char *result = global::GlobalPacket::GetPacket()->Serialize(request, size);
  if(result == NULL) {
    global::LogError("%s:%d (%s) 消息序列化失败 size=%lu",
        __FILE__, __LINE__, __FUNCTION__, size);
    return -1;
  }
  // 设置结果
  this->result_.assign(result, size);

  return 0;
}

typedef boost::shared_ptr<RequestProxy> RequestProxyPtr;

}  // namespace python

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PYTHON__REQUEST__PROXY__H


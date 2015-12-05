//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-17 13:56:47.
// File name: request_soul_operate.h
//
// Description:
// Define class RequestSoulOperate.
//

#ifndef __GAME__SERVER__SOUL__REQUEST__SOUL__OPERATE__H
#define __GAME__SERVER__SOUL__REQUEST__SOUL__OPERATE__H

#include <stddef.h>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace soul {

class RequestSoulOperate : public core::Noncopyable {
 public:
  RequestSoulOperate();
  ~RequestSoulOperate();

  bool Initialize();
  void Finalize();

 private:
  // 是否存在
  int OnRequestCheckExist(void *message, size_t size);
  // 增加英灵
  int OnRequestAddSoul(void *message, size_t size);
  // 获取玩家英灵基本信息
  int OnRequestSoulGet(void *message, size_t size);
  // 获取玩家能量状态
  int RequestSoulGetEnergyStatus(void *message, size_t size);
};

}  // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__REQUEST__SOUL__OPERATE__H


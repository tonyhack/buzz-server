//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-22 10:50:28.
// File name: facade_request.h
//
// Description:
// Define class FacadeRequest.
//

#ifndef __GAME__SERVER__PLAYING__FACADE__REQUEST__H
#define __GAME__SERVER__PLAYING__FACADE__REQUEST__H

#include "core/base/types.h"
#include "entity/item_types.h"
#include "global/singleton_factory.h"

namespace game {

namespace server {

namespace playing {

class FacadeRequest : public global::SingletonFactory<FacadeRequest> {
  friend class global::SingletonFactory<FacadeRequest>;

 public:
  // 创建场景地图
  core::uint64 CreateSceneMap(core::uint32 map);

  // 销毁场景地图
  bool DestorySceneMap(core::uint64 scene);

  // 检查背包空位
  bool CheckAddPacketItem(core::uint64 actor, core::uint32 item_template,
      core::uint32 number, entity::BindRequireType::type bind);

  // 背包增加物品
  bool AddPacketItem(core::uint64 actor, core::uint32 item_template,
      core::uint32 number, entity::BindRequireType::type bind);

  // 给角色增加一个BUFF
  bool AddBuff(core::uint64 actor, core::uint32 buff);

  // 检测是否在护送状态
  bool CheckTransport(core::uint64 actor);

 private:
  FacadeRequest() {}
  ~FacadeRequest() {}
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__FACADE__REQUEST__H


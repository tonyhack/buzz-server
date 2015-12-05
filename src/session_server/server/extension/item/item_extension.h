#ifndef __SESSION__SERVER__ITEM__ITEM__EXTENSION__H
#define __SESSION__SERVER__ITEM__ITEM__EXTENSION__H

#include <vector>
#include <string>

#include "entity/mall_types.h"
#include "global/singleton_factory.h"
#include "session_server/server/extension/item/actor_login_event_handler.h"
#include "session_server/server/extension/item/game_message_handler.h"
#include "session_server/server/extension/item/request_item_handler.h"
#include "session_server/server/extension/item/system_event_handler.h"

namespace session {

namespace server {

namespace item {

class ActorLoginEventHandler;

class ItemExtension : public global::SingletonFactory<ItemExtension> {
  typedef std::vector<std::string> TreasureInfoVec;
  typedef std::vector<entity::MallTreasureCell> MallTreasureVec;

 public:
  ItemExtension();
  ~ItemExtension();

  bool Initialize();
  void Finalize();

  void Start();
  void Stop();

  void AddTreasureInfo(std::string &info);

  void SendTreasureInfo(core::uint64 actor);

  // 向商城配置文件中加入一行数据
  bool PushMallTreasureData(const entity::MallTreasureCell &cell);
  // 同步给游戏服务器商城配置
  void SynchronizeMallConfig(core::int32 game_id);
  // 同步给所有的游戏服务器及玩家(当商城配置发生变化)
  void SynchronizeMallTreasureData();
  // 同步商城数据给单独的玩家(玩家上线的时候)
  void SynchronizeMallTreasureDataActor(core::uint64 actor);
  // 清除商城配置(没有同步给游戏服务器和玩家)
  void ClearMallConfigure();

 private:
  GameMessageHandler game_message_handler_;
  ActorLoginEventHandler actor_login_;
  SystemEventHandler system_event_;

  RequestItemHandler request_item_;

  TreasureInfoVec world_treasure_vec_;
  MallTreasureVec mall_treasure_vec_;
};

} // namespace item

} // namespace server

} // namespace session

#endif // __SESSION__SERVER__ITEM__ITEM__EXTENSION__H


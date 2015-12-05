//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-28 17:51:23.
// File name: game_entity.h
//
// Description:
// Define class GameEntity.
//

#ifndef __GAME__SERVER__GAME__ENTITY__H
#define __GAME__SERVER__GAME__ENTITY__H

#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "entity/entity_types.h"
#include "game_server/server/game_object.h"
#include "game_server/server/game_server.h"
#include "game_server/server/request/game_scene_request_types.h"
#include "global/global_packet.h"
#include "global/logging.h"

namespace game {

namespace server {

class EntityKey {
 public:
  EntityKey() {}
  ~EntityKey() {}

  entity::EntityType::type type_;
  core::uint64 id_;
};

class GameEntity : public GameObject {
  typedef std::vector<int32_t> AttributeFields;

  typedef void (*AttributeReviseFunctor)(core::int32 &value);

 public:
  GameEntity();
  virtual ~GameEntity();

  void Allocate();
  void Deallocate();

  void Initialize();
  void Finalize();

  // 获取属性集
  inline const AttributeFields &GetEntityClientAttributes() const {
    return this->client_attribute_;
  }
  inline const AttributeFields &GetEntityServerAttributes() const {
    return this->server_attribute_;
  }

  // 私有可见属性
  core::int32 GetAttribute(entity::EntityClientFields::type type) const;
  bool SetAttribute(entity::EntityClientFields::type type,
      core::int32 value, bool synchronize = true);

  // 服务器端属性
  core::int32 GetAttribute(entity::EntityServerFields::type type) const;
  bool SetAttribute(entity::EntityServerFields::type type,
      core::int32 value, bool synchronize = true);

  // 得到实体类型
  virtual entity::EntityType::type GetType() const = 0;

  // 同步属性到客户端
  virtual void Synchronize() = 0;
  virtual void Synchronize(entity::EntityClientFields::type type) = 0;

  // 场景
  inline void SetScene(core::uint64 scene) {
    this->scene_ = scene;
  }
  inline core::uint64 GetScene() const {
    return this->scene_;
  }

  // 坐标
  inline void SetPosition(core::uint32 x, core::uint32 y) {
    this->x_ = x;
    this->y_ = y;
  }
  inline void GetPosition(core::uint32 &x, core::uint32 &y) const {
    x = this->x_;
    y = this->y_;
  }
  inline core::uint32 GetPosx() const {
    return this->x_;
  }
  inline core::uint32 GetPosy() const {
    return this->y_;
  }

  // 坐标格子偏移
  inline void SetPosOffset(core::uint8 offset_x, core::uint8 offset_y) {
    this->offset_x_ = offset_x;
    this->offset_y_ = offset_y;
  }
  inline void GetPosOffset(core::uint8 &offset_x, core::uint8 &offset_y) const {
    offset_x = this->offset_x_;
    offset_y = this->offset_y_;
  }

  // 发送消息到周围
  template <typename T>
  void SendMessageAround(const T &message, gateway::protocol::MessageType::type type,
      bool self = false);

 protected:
  // 修正属性
  void ReviseAttribute(entity::EntityClientFields::type type);
  void ReviseAttribute(entity::EntityServerFields::type type);

  // 私有可见属性(同步给客户端)
  AttributeFields client_attribute_;
  // 服务器端属性(客户端不可见)
  AttributeFields server_attribute_;

  // 场景与坐标
  core::uint64 scene_;
  core::uint32 x_;
  core::uint32 y_;

  // 坐标(格子)偏移
  core::uint8 offset_x_;
  core::uint8 offset_y_;
};

template <typename T>
void GameEntity::SendMessageAround(const T &message,
    gateway::protocol::MessageType::type type, bool self) {
  // 获取当前玩家周围的玩家列表
  request::RequestSceneGetAroundActors request;
  request.__set_type_(this->GetType());
  request.__set_id_(this->GetID());
  int ret_request = ExtensionManager::GetInstance()->Request(
      request::RequestType::REQUEST_SCENE_GET_AROUND_ACTORS, &request, sizeof(request));
  if(ret_request != 0) {
    global::LogError("%s:%d (%s) 请求获取周围玩家列表失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  // 玩家对象需要给自己发送时
  if(self && this->GetType() == entity::EntityType::TYPE_ACTOR) {
    request.actors_.push_back(this->GetID());
  }
  // 周围是否没有玩家
  if(request.actors_.empty()) {
    return ;
  }
  // 多播给列表玩家
  size_t size = 0;
  const char *message2 = global::GlobalPacket::GetPacket()->Serialize(message, size);
  if(message2 == NULL) {
    global::LogError("%s:%d (%s) 序列化消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::MessageUserMessageMulticast multicast;
  multicast.__set_type_(type);
  multicast.message_.assign(message2, size);
  size_t pos = 0;
  for(; pos < request.actors_.size(); ++pos) {
    multicast.actors_.push_back(request.actors_[pos]);
  }
  GameServerSingleton::GetInstance().BroadcastGateway(multicast,
      protocol::MessageType::MESSAGE_USER_MESSAGE_MULTICAST);
}

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GAME__ENTITY__H


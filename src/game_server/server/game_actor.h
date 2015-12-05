//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-25 18:06:36.
// File name: game_actor.h
//
// Description:
// Define class GameActor.
//

#ifndef __GAME__SERVER__GAME__ACTOR__H
#define __GAME__SERVER__GAME__ACTOR__H

#include <vector>

#include "core/timer_handler.h"
#include "core/base/types.h"
#include "database_server/protocol/storage_actor_login_types.h"
#include "entity/actor_types.h"
#include "entity/entity_types.h"
#include "entity/fight_score_types.h"
#include "entity/functionality_types.h"
#include "entity/resource_types.h"
#include "entity/vip_types.h"
#include "game_server/protocol/game_protocol_types.h"
#include "game_server/server/actor_attribute_update.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_role.h"
#include "game_server/server/game_terminal.h"
#include "game_server/server/load_progress.h"
#include "gateway_server/protocol/gateway_protocol_types.h"

namespace game {

namespace server {

class GameActor : public GameRole, public core::TimerHandler {
  enum {
    TIMER_ID_SAVE = 0,
    TIMER_NAME_COLOR,
    TIMER_VIP_TIME,
    TIMER_FCM_HALF_INCOME,
    TIMER_FCM_ZERO_INCOME,
  };

  typedef std::vector<int32_t> AttributeFields;
  typedef std::vector<core::uint8> FunctionalityStates;
  typedef std::vector<core::int32> SubFightScores;

  typedef void (*AttributeReviseFunctor)(core::int32 &value);

 public:
  using GameRole::SetAttribute;
  using GameRole::GetAttribute;

  GameActor();
  virtual ~GameActor();

  void Allocate();
  void Deallocate();

  bool Initialize(const database::protocol::StorageActorField &fields);
  void Finalize();

  void RequireLogout();

  // 加载结束
  void OnExtentionLoadFinish();

  // 玩家所在网关ID
  inline void SetGateway(core::uint32 gateway) {
    this->gateway_ = gateway;
  }
  inline core::uint32 GetGateway() const {
    return this->gateway_;
  }

  // 玩家名称的get/set.
  inline const std::string &GetName() const {
    return this->name_;
  }
  inline void SetName(const std::string &name) {
    this->name_ = name;
  }

  // 玩家角色ID的get/set.
  inline core::uint64 GetActorID() const {
    return GameRole::GetID();
  }
  inline void SetActorID(core::uint64 id) {
    GameRole::SetID(id);
  }

  // 获取玩家选中目标的get/set.
  inline const EntityKey &GetSelectTarget() const {
    return this->target_;
  }

  inline void SetSelectTarget(entity::EntityType::type type, core::uint64 id) {
    this->target_.type_ = type;
    this->target_.id_ = id;
  }

  // 获取/设置上次登出时间
  inline time_t GetLastLogoutTime() const {
    return this->last_logout_time_;
  }
  inline void SetLastLogoutTime(time_t time) {
    this->last_logout_time_ = time;
  }
  inline time_t GetCreateTime() const {
    return this->create_time_;
  }

  // 获取属性集
  inline const AttributeFields &GetActorAoiAttributes() const {
    return this->aoi_attribute_;
  }
  inline const AttributeFields &GetActorClientAttributes() const {
    return this->client_attribute_;
  }
  inline const AttributeFields &GetActorServerAttributes() const {
    return this->server_attribute_;
  }

  // 覆盖父类方法
  bool SetAttribute(entity::RoleAoiFields::type type,
      core::int32 value, bool synchronize = true);

  // Aoi属性
  core::int32 GetAttribute(entity::ActorAoiFields::type type) const;
  bool SetAttribute(entity::ActorAoiFields::type type,
      core::int32 value, bool synchronize = true);

  // 私有可见属性
  core::int32 GetAttribute(entity::ActorClientFields::type type) const;
  bool SetAttribute(entity::ActorClientFields::type type,
      core::int32 value, bool synchronize = true);

  // 服务器端属性
  core::int32 GetAttribute(entity::ActorServerFields::type type) const;
  bool SetAttribute(entity::ActorServerFields::type type,
      core::int32 value, bool synchronize = true);

  // 检查系统功能是否开启
  bool CheckFunctionalityState(entity::FunctionalityType::type type) const;
  void SetFunctionalityState(entity::FunctionalityType::type type,
      bool state = true, bool synchronize = true);
  std::string GetFunctionalityState() const;

  // 子战斗力
  core::int32 GetSubFightScore(entity::SubFightScoreType::type type) const;
  void SetSubFightScore(entity::SubFightScoreType::type type, core::int32 score,
      bool synchronize = true);
  // 战斗力
  core::int32 GetFightScore(entity::FightScoreType::type type) const;
  core::int32 GetCurrentFightScore() const;

  // 得到加载进度
  inline LoadProgress &GetLoadProgress() {
    return this->load_progress_;
  }

  // 获取资源数量
  core::int32 GetResource(entity::ResourceID::type type) const;
  // 添加(非道具)资源
  void AddResource(entity::ResourceID::type type, core::int32 value);

  // 获取/设置 跳转登陆状态
  inline bool GetJumpLogin() const {
    return this->jump_login_;
  }
  inline void SetJumpLogin(bool jump) {
    this->jump_login_ = jump;
  }

  // 前一个所在地图、坐标
  inline void SetPreMap(core::uint32 map) {
    this->pre_map_ = map;
  }
  inline core::uint32 GetPreMap() const {
    return this->pre_map_;
  }
  inline void SetPrePosition(core::uint32 x, core::uint32 y) {
    this->pre_x_ = x;
    this->pre_y_ = y;
  }
  inline core::uint32 GetPrePosx() const {
    return this->pre_x_;
  }
  inline core::uint32 GetPrePosy() const {
    return this->pre_y_;
  }

  inline void SetBrownName(bool synchronize = true) {
    this->SetAttribute(entity::ActorAoiFields::BROWN_NAME, kBrownNameValue_, synchronize);
  }
  inline bool GetBrownName() const {
    return this->GetAttribute(entity::ActorAoiFields::BROWN_NAME) != 0;
  }

  void OpenFunctionality(core::int32 src_level, core::int32 dest_level);
  
  // 加VIP
  bool AlterVip(entity::VipType::type type, core::int32 vip_time, bool sync);
  // 同步VIP剩余时间
  void SynchronizeVip();

  // 世界地图跳转
  inline core::int32 GetWorldJumpVipNum() const {
    return this->world_jump_vip_Num_;
  }
  void SetWorldJumpVipNum(core::int32 num) {
    this->world_jump_vip_Num_ = num;
  }

  // 防沉迷
  inline void SetFcmOnlineTime(core::int32 fcm_online_time) {
    this->fcm_online_time_ = fcm_online_time;
  }

  // 使用护符
  bool UseHpTasliman();

  bool UseMpTasliman();

  // 增加护符值
  bool AddHpTasliman(core::int32 value);

  bool AddMpTasliman(core::int32 value);

  // 发送消息
  template <typename T>
  void SendMessage(const T &message, gateway::protocol::MessageType::type type);

  // 类型
  virtual entity::EntityType::type GetType() const;

  // 同步属性到AOI/客户端
  virtual void Synchronize();
  virtual void Synchronize(entity::ActorAoiFields::type type);
  virtual void Synchronize(entity::ActorClientFields::type type);
  virtual void Synchronize(entity::RoleAoiFields::type type);
  virtual void Synchronize(entity::RoleClientFields::type type);
  virtual void Synchronize(entity::EntityClientFields::type type);

  virtual void OnTimer(core::uint32 id);

  void DailyClean(core::int32 days);

 private:
  // 修正属性
  void ReviseAttribute(entity::ActorAoiFields::type type);
  void ReviseAttribute(entity::ActorClientFields::type type);
  void ReviseAttribute(entity::ActorServerFields::type type);

  void Offline();
  void Save();
  void CheckNameColor();
  void VipExpired();
  void AddExp(core::int32 add_value);

  core::int32 GetCommonPartFightScore() const;
  void UpdateActorBaseFightScore(bool synchronize = true);

  bool InitializeFcm(time_t now);

  // 所在网关服务器ID
  core::uint32 gateway_;

  // Area of interesting(公共可见属性，需要广播)
  AttributeFields aoi_attribute_;
  // core::uint32 aoi_attribute_[entity::ActorAoiFields::AOI_END];
  // 私有可见属性(同步给客户端)
  AttributeFields client_attribute_;
  // core::uint32 client_attribute_[entity::ActorClientFields::CLIENT_END];
  // 服务器端属性(客户端不可见)
  AttributeFields server_attribute_;
  // core::uint32 server_attribute_[entity::ActorServerFields::SERVER_END];

  std::string name_;

  // 加载缓存生命值、魔法值
  core::uint32 load_current_hp_;
  core::uint32 load_current_mp_;

  // 选中目标信息
  EntityKey target_;

  // 上次登出时间
  time_t last_logout_time_;
  // 本次登陆时间
  time_t login_time_;
  // 创建时间
  time_t create_time_;

  // 系统功能开启状态
  FunctionalityStates func_states_;

  // 子战斗力
  SubFightScores sub_fight_scores_;

  // 加载进度
  LoadProgress load_progress_;

  // 属性更新
  ActorAttributeUpdate actor_attribute_update_;

  // 跳转登陆状态
  bool jump_login_;

  // 前一个所在地图(只能是静态地图)与坐标
  core::uint32 pre_map_;
  core::uint32 pre_x_;
  core::uint32 pre_y_;

  // 到期日期
  time_t vip_expire_time_;

  // 世界地图跳转VIP次数
  core::int32 world_jump_vip_Num_;

  // 防沉迷累计在线时间
  core::int32 fcm_online_time_;

  // 上次血护符使用时间
  time_t last_hp_talisman_use_time_;
  // 上次蓝护符使用时间
  time_t last_mp_talisman_use_time_;
  // 血护符当前值
  time_t current_hp_talisman_value_;
  // 蓝护符当前值
  time_t current_mp_talisman_value_;

  static const core::int32 kBrownNameValue_ = 3;
};

template <typename T>
void GameActor::SendMessage(const T &message,
    gateway::protocol::MessageType::type type) {
  global::ThriftPacket *thrift = GameServerSingleton::GetInstance().GetThrift();
  if(thrift == NULL) {
    global::LogError("%s:%d (%s) Thrift is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  GameTerminal *terminal = 
    GameServerSingleton::GetInstance().GetGatewayTerminal(this->gateway_);
  if(terminal == NULL) {
    global::LogError("%s:%d (%s) Get gateway[%d] terminal failed.",
        __FILE__, __LINE__, __FUNCTION__, this->gateway_);
    return ;
  }
  size_t size = 0;
  const char *message1 = thrift->Serialize(message, size);
  if(message1 == NULL) {
    global::LogError("%s:%d (%s) Deserialize message failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if (size >= 40000) {
    LOG_FATAL("message_size=[%zd], message_type=[%d].", size, type);
  }

  protocol::MessageUserMessageForward forward;
  forward.__set_type_(type);
  forward.__set_actor_(this->GetActorID());
  forward.message_.assign(message1, size);
  terminal->SendMessage(forward, protocol::MessageType::MESSAGE_USER_MESSAGE_FORWARD);
}

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__GAME__ACTOR__H


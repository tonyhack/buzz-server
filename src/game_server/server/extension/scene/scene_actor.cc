//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-03-12 10:01:05.
// File name: scene_actor.cc
//
// Description:
// Define class SceneActor.
//

#include "game_server/server/extension/scene/scene_actor.h"

#include "entity/inform_types.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/extension/scene/configure.h"
#include "game_server/server/extension/scene/scene.h"
#include "game_server/server/extension/scene/scene_npc.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/configure.h"
#include "gateway_server/protocol/gateway_inform_protocol_types.h"
#include "gateway_server/protocol/gateway_scene_protocol_types.h"
#include "global/common_functions.h"
#include "global/server_configure.h"
#include "global/configure/configure.h"

namespace game {

namespace server {

namespace scene {

SceneActor::SceneActor() : scene_(NULL),
  actor_(NULL), move_count_(0), speed_(0) {}
SceneActor::~SceneActor() {}

bool SceneActor::Initialize(GameActor *actor, Scene *scene) {
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 初始化失败，参数为NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  this->scene_ = scene;
  this->actor_ = actor;
  if(this->zazen_.Initialize(this) == false) {
    LOG_ERROR("打坐初始化失败");
    return false;
  }
  this->gather_npc_id_ = 0;
  this->ResetCheck();

  this->consecutive_kill_num_ = 0;
  this->last_consecutive_time_ = 0;
  this->pvp_fighting_status_ = false;

  return true;
}

void SceneActor::Finalize() {
  // 结束时删除复活定时器
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_ID_REVIVE, this);
  // 删除连斩定时器
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_ID_CONSECUTIVE_KILL, this);
  this->zazen_.Finalize();
  this->last_consecutive_time_ = 0;
  this->consecutive_kill_num_ = 0;
  for(size_t i = 0; i < this->consecutive_kill_buffs_.size(); ++i) {
    request::RequestBuffEnd request;
    request.__set_dest_entity_type_(entity::EntityType::TYPE_ACTOR);
    request.__set_dest_entity_id_(this->actor_->GetActorID());
    request.__set_buff_id_(this->consecutive_kill_buffs_[i]);
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_BUFF_END, &request, sizeof(request)) == -1 || 
        request.succeed_ == false) {
      LOG_ERROR("删除BUFF(%d)失败", this->consecutive_kill_buffs_[i]);
    }
  }
  this->consecutive_kill_buffs_.clear();
}

bool SceneActor::CheckMove() {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return false;
  }

  if (this->actor_->GetBasicStatus() == entity::RoleBasicStatus::STATUS_DIE) {
    return false;
  }

  // 判断是否需要检查移动频率
  if (global::ServerConfigureSingleton().GetInstance().CheckMoveCheckEnable() == false) {
    return true;
  }

  if (this->move_count_ + 1 == SceneActor::kCheckFrequency_) {
    core::int32 move_time = this->total_move_time_ + this->last_move_timestamp_.Distance(
        core::TimestampMillisecond(GlobalTimeTick::GetInstance()->TimeTick()));
    if (move_time < this->total_standard_time_) {
      return false;
    }
  }

  return true;
}

void SceneActor::MoveCount(bool right_angle) {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return;
  }

  // 得到当前时间
  core::TimestampMillisecond now(GlobalTimeTick::GetInstance()->TimeTick());

  // 减去边界移动时间(要从环形数组中移除的)
  this->total_move_time_ -= this->move_time_[this->move_count_];
  // 存储本次移动时间
  this->move_time_[this->move_count_] = this->last_move_timestamp_.Distance(now);
  // 加上本次移动时间
  this->total_move_time_ += this->move_time_[this->move_count_];

  // 更新上次移动时间戳为本次的
  this->last_move_timestamp_ = now;

  this->total_standard_time_ -= this->standard_time_[this->move_count_];
  if (right_angle) {
    core::int32 standard_spend_time = 1000 * game::server::Configure::kGridPixelWidth_ /
      this->actor_->GetAttribute(entity::RoleAoiFields::MOVE_SPEED);
    this->standard_time_[this->move_count_] = standard_spend_time;
  } else {
    core::int32 standard_spend_time = 1414 * game::server::Configure::kGridPixelWidth_ /
      this->actor_->GetAttribute(entity::RoleAoiFields::MOVE_SPEED);
    this->standard_time_[this->move_count_] = standard_spend_time;
  }
  this->total_standard_time_ += this->standard_time_[this->move_count_];

  // 环形数组向后移动
  if (this->move_count_ >= SceneActor::kCheckFrequency_ - 1) {
    this->move_count_ = 0;
  } else {
    ++this->move_count_;
  }
}

void SceneActor::ResetCheck() {
  // 重置 move_time_
  memset(this->move_time_, 0, sizeof(this->move_time_));
  memset(this->standard_time_, 0, sizeof(this->standard_time_));

  // 数据重置
  this->move_count_ = 0;
  this->total_move_time_ = 0;
  this->total_standard_time_ = 0;

  // 重置 last_move_timestamp_
  this->last_move_timestamp_ = core::TimestampMillisecond(
      GlobalTimeTick::GetInstance()->TimeTick());
}

void SceneActor::OnDeath() {
  if(this->scene_ == NULL) {
    global::LogError("%s:%d (%s) 场景玩家[%lu] 所在场景不存在",
        __FILE__, __LINE__, __FUNCTION__, this->GetActorID());
    return ;
  }

  const ServerMapConfigure *server_map_configure =
    Configure::GetInstance()->GetServerMapConfigure();
  if(server_map_configure == NULL) {
    global::LogError("%s:%d (%s) 找不到配置 ServerMapConfigure",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  const MapInfo *map_info = server_map_configure->GetMapInfo(this->scene_->GetMapID());
  if(map_info == NULL) {
    global::LogError("%s:%d (%s) 找不到地图[%d]信息配置",
        __FILE__, __LINE__, __FUNCTION__, this->scene_->GetMapID());
    return ;
  }

  // 获取角色阵营
  GameActor *game_actor = this->GetActor();
  if(game_actor == NULL) {
    LOG_ERROR("角色没有找到");
    return ;
  }
  const ReviveAreaInfo *revive = map_info->GetReviveArea(
      game_actor->GetAttribute(entity::RoleAoiFields::CAMP));
  if(revive == NULL) {
    LOG_ERROR("复活点camp[%d]配置错误", game_actor->GetAttribute(
          entity::RoleAoiFields::CAMP));
    return ;
  }
  // 定时器: 自动复活时间
  GameServerSingleton::GetInstance().AddTimer(TIMER_ID_REVIVE,
      revive->duration_ * 1000, 1, this, "SceneActor::Death");
}

bool SceneActor::Revive(bool relocation) {
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_ID_REVIVE, this);

  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) 角色指针为NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->scene_ == NULL) {
    global::LogError("%s:%d (%s) 场景玩家[%lu] 所在场景不存在，复活失败",
        __FILE__, __LINE__, __FUNCTION__, this->GetActorID());
    return false;
  }

  if(this->actor_->GetBasicStatus() != entity::RoleBasicStatus::STATUS_DIE) {
    global::LogError("%s:%d (%s) 场景玩家[%lu] 复活时不是死亡状态，复活失败",
        __FILE__, __LINE__, __FUNCTION__, this->GetActorID());
    return false;
  }

  // 状态设置为普通状态
  this->actor_->SetBasicStatus(entity::RoleBasicStatus::STATUS_COMMON);

  // 设置血量
  this->actor_->SetAttribute(entity::RoleAoiFields::CURRENT_HP,
      this->actor_->GetAttribute(entity::RoleAoiFields::MAX_HP), true);
  this->actor_->SetAttribute(entity::RoleAoiFields::CURRENT_MP,
      this->actor_->GetAttribute(entity::RoleAoiFields::MAX_MP), true);

  // 如果需要回复活点，则跳转
  if(relocation == true) {
    const ServerMapConfigure *server_map_configure =
      Configure::GetInstance()->GetServerMapConfigure();
    if(server_map_configure == NULL) {
      global::LogError("%s:%d (%s) 找不到配置 ServerMapConfigure",
          __FILE__, __LINE__, __FUNCTION__);
      return false;
    }

    const MapInfo *map_info = server_map_configure->GetMapInfo(this->scene_->GetMapID());
    if(map_info == NULL) {
      global::LogError("%s:%d (%s) 找不到地图[%d]信息配置",
          __FILE__, __LINE__, __FUNCTION__, this->scene_->GetMapID());
      return false;
    }
    const ReviveAreaInfo *revive_info = map_info->GetReviveArea(
        this->actor_->GetAttribute(entity::RoleAoiFields::CAMP));
    if(revive_info == NULL) {
      LOG_ERROR("复活点配置没有找到[%d]", this->actor_->GetAttribute(
            entity::RoleAoiFields::CAMP));
      return false;
    }
    core::int32 add_x = 0;
    core::int32 add_y = 0;
    if(revive_info->width_ != 0) {
      add_x = ExtensionManager::GetInstance()->GetRandom().Random(revive_info->width_);
    }
    if(revive_info->heigth_ != 0) {
      add_y = ExtensionManager::GetInstance()->GetRandom().Random(revive_info->heigth_);
    }
    add_x -= revive_info->width_/2;
    add_y -= revive_info->heigth_/2;
    add_x += revive_info->x_;
    add_y += revive_info->y_;
    if(revive_info->map_ == 0) {
      this->scene_->JumpScene(this->actor_, this->scene_->GetSceneID(), add_x, add_y);
    } else {
      this->scene_->JumpMap(this->actor_, revive_info->map_, add_x, add_y);
    }
  }
  return true;
}

void SceneActor::AfterKilled(SceneActor* attacker) {
  if(attacker == NULL || this->actor_ == NULL) {
    LOG_ERROR("参数错误");
    return ;
  }

  // 是否PK惩罚
  if(this->scene_->IsPunishForPK() && this->actor_->GetBrownName() == false) {
    request::RequestBuffStart request; 
    request.__set_buff_id_(MISC_CONF()->pk_buff_id_);
    request.__set_src_entity_type_(attacker->GetType());
    request.__set_src_entity_id_(attacker->GetActorID());
    request.__set_dest_entity_type_(attacker->GetType());
    request.__set_dest_entity_id_(attacker->GetActorID());
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_BUFF_START, &request, sizeof(request)) != 0) {
      global::LogError("%s:%d (%s) 请求开启buff失败",
          __FILE__, __LINE__, __FUNCTION__); 
    }
  }

  // 是否死亡保护
  if(this->scene_->IsDieProtect() && this->actor_->GetBrownName() == false) {
    request::RequestBuffStart request; 
    request.__set_buff_id_(MISC_CONF()->protect_buff_id_);
    request.__set_src_entity_type_(this->actor_->GetType());
    request.__set_src_entity_id_(GetActorID());
    request.__set_dest_entity_type_(this->actor_->GetType());
    request.__set_dest_entity_id_(GetActorID());
    if(ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_BUFF_START, &request, sizeof(request)) != 0) {
      global::LogError("%s:%d (%s) 请求开启buff失败",
          __FILE__, __LINE__, __FUNCTION__); 
    }
  }

  // 发送提示消息
  gateway::protocol::MessageSceneRoleKilled message;
  message.__set_attacker_type_(entity::EntityType::TYPE_ACTOR);
  message.__set_attacker_id_(global::ToString(attacker->GetActorID()));
  message.__set_attacker_name_(attacker->GetActorName());
  this->actor_->SendMessage(message, gateway::protocol::MessageType::MESSAGE_SCENE_ROLE_KILLED);

  // 公告
  gateway::protocol::MessageInformNotify inform;
  inform.__set_id_(entity::InformType::SKILL_KILLED_BY_ACTOR);
  inform.params_.push_back(global::ToString(attacker->GetActorID()));
  inform.params_.push_back(attacker->GetActorName());
  this->actor_->SendMessage(inform, gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
}

void SceneActor::AfterKilledByNpc(SceneNpc* scene_npc) {
  if(scene_npc == NULL || this->actor_ == NULL) {
    return ;
  }
  GameNpc* npc = scene_npc->GetGameNpc();
  if(npc == NULL) {
    return ;
  }

  // 发送提示消息
  gateway::protocol::MessageSceneRoleKilled message;
  message.__set_attacker_type_(entity::EntityType::TYPE_NPC);
  message.__set_attacker_id_(npc->GetStringID());
  message.__set_template_id_(npc->GetTemplateID());
  this->actor_->SendMessage(message, gateway::protocol::MessageType::MESSAGE_SCENE_ROLE_KILLED);

  // 公告
  gateway::protocol::MessageInformNotify inform;
  inform.__set_id_(entity::InformType::SKILL_KILLED_BY_NPC);
  inform.params_.push_back(global::ToString(npc->GetTemplateID()));
  this->actor_->SendMessage(inform, gateway::protocol::MessageType::MESSAGE_INFORM_NOTIFY);
}

bool SceneActor::ChooseReviveMode(entity::ReviveType::type type, bool buy) {

  if(this->actor_ == NULL) {
    global::LogError("%s:%d (%s) 角色指针为NULL",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->scene_ == NULL) {
    global::LogError("%s:%d (%s) 场景玩家[%lu] 所在场景不存在，复活失败",
        __FILE__, __LINE__, __FUNCTION__, this->GetActorID());
    return false;
  }

  if(this->actor_->GetBasicStatus() != entity::RoleBasicStatus::STATUS_DIE) {
    global::LogError("%s:%d (%s) 场景玩家[%lu] 复活时不是死亡状态，复活失败",
        __FILE__, __LINE__, __FUNCTION__, this->GetActorID());
    return false;
  }

  if(type == entity::ReviveType::STAY) {
    // 是否允许原地复活
    if(this->scene_->IsReviveStay()) {
      // 检测是否有复活道具
      request::RequestRemoveItem request;
      request.__set_actor_id_(this->GetActorID());
      request.__set_item_template_(MISC_CONF()->revive_item_id_);
      request.__set_number_(1);
      request.__set_bind_(entity::BindRequireType::BOTH);
      int ret_request = ExtensionManager::GetInstance()->Request(
          request::RequestType::REQUEST_REMOVE_ITEM, &request, sizeof(request));
      if(ret_request != 0) {
        global::LogError("%s:%d (%s) 请求删除道具失败",
            __FILE__, __LINE__, __FUNCTION__); 
        return false;
      }
      // 删除成功
      if(request.remove_number_ == 0) {
        return false;
      } 
    } else {
      return false;
    }
  }

  bool result = false;
  switch(type) {
    case entity::ReviveType::SPAWN_POINT: // 复活点
      result = this->Revive(true);
      break;  
    case entity::ReviveType::STAY:        // 原地      
      result = this->Revive(false);
      break;
    default:
      break;
  }
  if(result == false) {
    return false;
  }

  // 广播复活事件
  event::EventActorRelive event;
  event.__set_actor_(this->actor_->GetActorID());
  event.__set_type_(type);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_RELIVE, this->actor_->GetActorID(),
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));

  return true;
}

void SceneActor::OnTimer(core::uint32 id) {
  if(id == TIMER_ID_REVIVE) {
    this->Revive(true);
  } else if(id == TIMER_ID_CONSECUTIVE_KILL) {
    this->OnConsecutiveKill();
  }
}

void SceneActor::KilledNpc(SceneNpc *scene_npc) {
  if(scene_npc == NULL) {
    LOG_ERROR("输入参数错误");
    return;
  }
  GameNpc *npc = scene_npc->GetGameNpc();
  if(npc == NULL) {
    LOG_ERROR("NPC 没有找到");
    return;
  }
  if(this->actor_ == NULL) {
    LOG_ERROR("玩家对象为空");
    return;
  }
  // 处理连斩
  if(abs(npc->GetAttribute(entity::RoleAoiFields::LEVEL) - this->actor_->GetAttribute(
          entity::RoleAoiFields::LEVEL)) <= MISC_CONF()->consecutive_kill_level_) {
    // 获取当前的时间
    time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
    // 获取配置
    const ConsecutiveKillCell *cell = Configure::GetInstance()->GetNearCell(this->consecutive_kill_num_ + 1);
    if(cell != NULL) {
      if(now - this->last_consecutive_time_ > cell->span_time_) {
        this->consecutive_kill_num_ = 1;
        GameServerSingleton::GetInstance().AddTimer(TIMER_ID_CONSECUTIVE_KILL,
            cell->span_time_ * 1000, -1, this, "SceneActor::Death");
      } else {
        this->consecutive_kill_num_ += 1;
      }
      this->last_consecutive_time_ = now;
      const ConsecutiveKillCell *cell_new = Configure::GetInstance()->GetCell(this->consecutive_kill_num_);
      if(cell_new != NULL) {
        // 请求加buff
        request::RequestBuffStart request;
        request.__set_src_entity_id_(this->actor_->GetActorID());
        request.__set_dest_entity_type_(entity::EntityType::TYPE_ACTOR);
        request.__set_dest_entity_id_(this->actor_->GetActorID());
        request.__set_buff_id_(cell_new->buff_id_);
        if(ExtensionManager::GetInstance()->Request(
              request::RequestType::REQUEST_BUFF_START, &request, sizeof(request)) == -1 || 
            request.succeed_ == false) {
          LOG_ERROR("添加BUFF(%d)失败", cell_new->buff_id_);
        } else {
          // 加入BUFF
          this->consecutive_kill_buffs_.push_back(cell_new->buff_id_);
        }
        // 删除定时器
        GameServerSingleton::GetInstance().RemoveTimer(TIMER_ID_CONSECUTIVE_KILL, this);
        // 添加新的定时器
        GameServerSingleton::GetInstance().AddTimer(TIMER_ID_CONSECUTIVE_KILL,
            cell->span_time_ * 1000, -1, this, "SceneActor::Death");
      }
      // 通知客户端
      gateway::protocol::MessageSceneConsecutiveKillNotice message;
      message.__set_kill_num_(this->consecutive_kill_num_);
      this->actor_->SendMessage(message,
          gateway::protocol::MessageType::MESSAGE_SCENE_CONSECUTIVE_KILL_NOTICE);
    } else {
      LOG_ERROR("配置没有找到(%d)", this->consecutive_kill_num_ + 1);
    }
  }
}

void SceneActor::OnConsecutiveKill() {
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  // 获取配置
  const ConsecutiveKillCell *cell = Configure::GetInstance()->GetNearCell(this->consecutive_kill_num_);
  bool del = false;
  if(cell == NULL) {
    LOG_ERROR("配置没有找到(%d)", this->consecutive_kill_num_);
    del = true;
  } else {
    if(now - this->last_consecutive_time_ > cell->span_time_) {
      del = true;
    }
  }
  if(del) {
    // 清除数据
    this->last_consecutive_time_ = 0;
    this->consecutive_kill_num_ = 0;
    for(size_t i = 0; i < this->consecutive_kill_buffs_.size(); ++i) {
      request::RequestBuffEnd request;
      request.__set_dest_entity_type_(entity::EntityType::TYPE_ACTOR);
      request.__set_dest_entity_id_(this->actor_->GetActorID());
      request.__set_buff_id_(this->consecutive_kill_buffs_[i]);
      if(ExtensionManager::GetInstance()->Request(
            request::RequestType::REQUEST_BUFF_END, &request, sizeof(request)) == -1 || 
          request.succeed_ == false) {
        LOG_ERROR("删除BUFF(%d)失败", this->consecutive_kill_buffs_[i]);
      }
    }
    this->consecutive_kill_buffs_.clear();
    GameServerSingleton::GetInstance().RemoveTimer(TIMER_ID_CONSECUTIVE_KILL, this);
  }
}

}  // namespace scene

}  // namespace server

}  // namespace game


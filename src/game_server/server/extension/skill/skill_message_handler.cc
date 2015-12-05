//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-06-07 16:01:19.
// File name: skill_message_handler.cc
//
// Description:
// Define class SkillMessageHandler.
//

#include "game_server/server/extension/skill/skill_message_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/skill/configure.h"
#include "game_server/server/extension/skill/skill_role.h"
#include "game_server/server/extension/skill/skill_role_manager.h"
#include "game_server/server/request/game_skill_request_types.h"
#include "game_server/server/configure.h"
#include "game_server/server/game_actor.h"
#include "gateway_server/protocol/gateway_skill_protocol_types.h"
#include "global/global_packet.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace skill {

SkillMessageHandler::SkillMessageHandler() {}
SkillMessageHandler::~SkillMessageHandler() {}

bool SkillMessageHandler::Initialize() {
  // 玩家技能操作
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SKILL_OPERATE,
      boost::bind(&SkillMessageHandler::OnMessageSkillOperate,
        this, _1, _2, _3));

  // 玩家选择PK模式
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ACTOR_CHOOSE_PK_MODE,
      boost::bind(&SkillMessageHandler::OnMessageActorChoosePKMode,
        this, _1, _2, _3));


  return true;
}

void SkillMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_SKILL_OPERATE);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_ACTOR_CHOOSE_PK_MODE);
}     

void SkillMessageHandler::OnMessageSkillOperate(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 从技能角色管理器中获取玩家指针
  SkillRole *actor = SkillRoleManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取玩家(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  GameRole *game_role = actor->GetGameRole();
  if(game_role == NULL) {
    LOG_ERROR("获取 GameRole(%lu) 失败");
    return ;
  }

  if(game_role->GetBasicStatus() == entity::RoleBasicStatus::STATUS_DIE) {
    LOG_ERROR("玩家(%lu) 处于死亡状态，不能使用技能", id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageSkillOperate message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(message.type_ != gateway::protocol::SkillOperateType::USE) {
    global::LogError("%s:%d (%s) 玩家(%lu) 技能操作类型错误",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  Skill *skill = actor->GetUseSkill(message.skill_id_);
  if(skill == NULL) {
    LOG_ERROR("获取 Skill(%d) 失败", message.skill_id_);
    return ;
  }

  const SkillCell *cell = skill::Configure::GetInstance()->GetSkill(
      message.skill_id_, skill->GetLevel());
  if(cell == NULL) {
    LOG_ERROR("获取 SkillCell(%d) 失败", message.skill_id_);
    return ;
  }

  request::RequestUseSkill request;
  request.__set_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_id_(id);
  request.__set_skill_id_(message.skill_id_);
  request.__set_target_type_(message.context_.target_type_);
  request.__set_target_id_(atol(message.context_.target_id_.c_str()));
  request.__set_x_(message.context_.x_);
  request.__set_y_(message.context_.y_);
  request.__set_direction_(message.context_.direction_);

  // 请求释放技能
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_USE_SKILL,
      &request, sizeof(request)) == -1) {
    global::LogError("%s:%d (%s) 玩家(%lu) 请求释放技能出错",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  if(request.result_ != request::SkillResultType::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%lu) 请求释放技能出错(%u)",
        __FILE__, __LINE__, __FUNCTION__, id, request.result_);
    return ;
  }

  // 设置PVP战斗
  if(cell->friend_type_ == FriendType::HURT &&
      message.context_.target_type_ == entity::EntityType::TYPE_ACTOR) {
    SkillRole *target_actor = SkillRoleManager::GetInstance()->GetActor(
        atol(message.context_.target_id_.c_str()));
    if(target_actor) {
      actor->GetFightingStatus().SetPvpFighting();
      target_actor->GetFightingStatus().SetPvpFighting();
    }
  }
}

void SkillMessageHandler::OnMessageActorChoosePKMode(core::uint64 id, 
  const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 从技能角色管理器中获取玩家指针
  SkillRole *actor = SkillRoleManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取玩家(%llu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  // 解析消息
  gateway::protocol::MessageActorChoosePKMode message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 不是玩家
  if(actor->GetType() != entity::EntityType::TYPE_ACTOR) {
    return ;
  }
 
  GameActor* game_actor = (GameActor*)actor->GetGameRole();
  if(game_actor == NULL) {
    return;
  }
  
  // 检测是否能设置pk模式
  if(actor->CheckChangePKMode() == false) {
    game_actor->Synchronize(entity::ActorClientFields::PK_MODE);
    return ;
  } 

  // 检测设置值
  core::int8 pkmode =  (core::int8)message.pk_mode_;
  core::int8 pkchoose = (core::int8)((message.pk_mode_ & 0x0000FF00) >> 8);
  switch(pkmode) {
  case entity::ActorPKModeType::PEACE:
      // 需要检测时可以使用
      // pkchoose &= entity::ActorPKChooseType::SELF_DEFENSE;
  break;
  case entity::ActorPKModeType::ATTACK:
      // 需要检测时可以使用 重置自动反击选项
      // pkchoose &= ~entity::ActorPKChooseType::SELF_DEFENSE;
  break;
  default:
    return ;
  }
  // 需要检测时可以使用
  // core::int32 pk_mode = (pkchoose << 8) + pkmode;
  // 设置PK模式修改属性
  game_actor->SetAttribute(entity::ActorClientFields::PK_MODE, message.pk_mode_);
  return ;
}

}  // namespace skill

}  // namespace server

}  // namespace game


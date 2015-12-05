//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-02-26 15:08:59.
// File name: game_actor.cc
//
// Description:
//

#include "game_server/server/game_actor.h"

#include "coresh/communicator.h"
#include "database_server/client/database_client.h"
#include "database_server/protocol/storage_actor_save_types.h"
#include "entity/camp_types.h"
#include "entity/vocation_types.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/according_level_configure.h"
#include "game_server/server/configure.h"
#include "game_server/server/event/game_actor_event_types.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/fight_score_calculator.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/game_actor_pool.h"
#include "game_server/server/game_server.h"
#include "game_server/server/global_time_tick.h"
#include "game_server/server/session_channel.h"
#include "gateway_server/protocol/gateway_entity_protocol_types.h"
#include "global/actor_id.h"
#include "global/common_functions.h"
#include "global/configure/configure.h"
#include "global/game_misc.h"
#include "global/logging.h"
#include "global/server_configure.h"
#include "global/configure/configure.h"
#include "global/configure/load_vip_configure.h"
#include "session_server/protocol/game_actor_protocol_types.h"
#include "session_server/protocol/game_guild_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"

namespace game {

namespace server {

GameActor::GameActor() : actor_attribute_update_(this) {}
GameActor::~GameActor() {}

void GameActor::Allocate() {
  GameRole::Allocate();
  this->aoi_attribute_.resize(entity::ActorAoiFields::AOI_END, 0);
  this->client_attribute_.resize(entity::ActorClientFields::CLIENT_END, 0);
  this->server_attribute_.resize(entity::ActorServerFields::SERVER_END, 0);
  this->actor_attribute_update_.Initialize();
  this->gateway_ = 0;
  this->last_logout_time_ = 0;
  this->func_states_.resize(entity::FunctionalityType::MAX, 0);
  this->sub_fight_scores_.resize(entity::SubFightScoreType::MAX, 0);
  this->jump_login_ = false;
  this->world_jump_vip_Num_ = 0;
}

void GameActor::Deallocate() {
  GameRole::Deallocate();
  this->actor_attribute_update_.Finalize();
  this->aoi_attribute_.assign(this->aoi_attribute_.size(), 0);
  this->client_attribute_.assign(this->client_attribute_.size(), 0);
  this->server_attribute_.assign(this->server_attribute_.size(), 0);
  this->func_states_.assign(this->func_states_.size(), 0);
  this->sub_fight_scores_.assign(this->sub_fight_scores_.size(), 0);
  this->world_jump_vip_Num_ = 0;
}

bool GameActor::Initialize(const database::protocol::StorageActorField &fields) {
  GameRole::Initialize();
  if(this->load_progress_.Initialize() == false) {
    global::LogError("%s:%d (%s) 初始化 LoadProgress 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  time_t now_time = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  // 设置属性
  this->SetLastLogoutTime(fields.last_logout_time_);
  this->login_time_ = now_time;
  this->create_time_ = fields.create_time_;

  this->SetName(fields.name_);
  this->SetAttribute(entity::RoleAoiFields::LEVEL, fields.level_, false);
  this->SetAttribute(entity::RoleAoiFields::VOCATION, fields.vocation_, false);
  this->SetAttribute(entity::RoleAoiFields::CAMP, entity::CampType::TYPE_NONE, false);
  this->SetAttribute(entity::RoleServerFields::PHYSIQUE_ADD_VALUE,
      Configure::kInitialActorAttr_, false);
  this->SetAttribute(entity::RoleServerFields::STRENGTH_ADD_VALUE,
      Configure::kInitialActorAttr_, false);
  this->SetAttribute(entity::RoleServerFields::INTELLIGENCE_ADD_VALUE,
      Configure::kInitialActorAttr_, false);
  this->SetAttribute(entity::RoleServerFields::AGILE_ADD_VALUE,
      Configure::kInitialActorAttr_, false);
  this->SetAttribute(entity::RoleServerFields::SPIRIT_ADD_VALUE,
      Configure::kInitialActorAttr_, false);
  this->SetAttribute(entity::ActorAoiFields::GENDER, fields.gender_, false);
  core::uint32 appearance = fields.head_ | (core::uint32)((core::uint8)fields.hair_ << 8);
  this->SetAttribute(entity::ActorAoiFields::APPEARANCE, appearance, false);
  this->SetAttribute(entity::ActorAoiFields::BROWN_NAME, fields.brown_name_, false);
  this->SetAttribute(entity::ActorAoiFields::VIP, fields.vip_type_, false);
  // this->SetAttribute(entity::RoleAoiFields::CURRENT_HP, fields.current_hp_, false);
  // this->SetAttribute(entity::RoleAoiFields::CURRENT_MP, fields.current_mp_, false);
  // 缓存从数据库读出的当前生命值、当前魔法值，等到玩家所有模块加载完成后，再设置到属性中
  this->load_current_hp_ = fields.current_hp_;
  this->load_current_mp_ = fields.current_mp_;
  this->SetAttribute(entity::ActorClientFields::CURRENT_EXP,
      fields.current_exp_, false);
  this->SetAttribute(entity::ActorClientFields::NIMBUS,
      fields.nimbus_, false);
  this->SetAttribute(entity::ActorClientFields::GOLD,
      fields.gold_, false);
  this->SetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS,
      fields.freedom_dollars_, false);
  this->SetAttribute(entity::ActorClientFields::RESTRICT_DOLLARS,
      fields.restrict_dollars_, false);
  this->SetAttribute(entity::ActorClientFields::HONOUR,
      fields.honour_, false);
  this->SetAttribute(entity::ActorClientFields::WAR_SOUL,
      fields.war_soul_, false);
  this->SetAttribute(entity::ActorClientFields::GEM_SHARD,
      fields.gem_shard_, false);
  this->SetAttribute(entity::ActorClientFields::JUSTICE,
      fields.justice_, false);
  this->SetAttribute(entity::ActorClientFields::PK_MODE,
      fields.pk_mode_, false);
  this->SetAttribute(entity::ActorClientFields::PACKET_CAPACITY,
      fields.packet_capacity_, false);
  this->SetAttribute(entity::ActorClientFields::STORAGE_CAPACITY,
      fields.storage_capacity_, false);
  this->SetAttribute(entity::ActorClientFields::MIRACLE_INTEGRAL,
      fields.miracle_integral_, false);
  this->SetAttribute(entity::ActorClientFields::LEGEND_INTEGRAL,
      fields.legend_integral_, false);
  this->SetAttribute(entity::ActorClientFields::ARENA_INTEGRAL,
      fields.arena_integral_, false);
  this->SetBasicStatus(entity::RoleBasicStatus::STATUS_COMMON, false);

  // 设置前一个地图位置
  this->SetPreMap(fields.pre_map_);
  this->SetPrePosition(fields.pre_x_, fields.pre_y_);

  // 设置世界跳转次数
  this->SetWorldJumpVipNum(fields.world_jump_vip_num_);

  // 系统功能开启状态
  for (size_t i = 0; i < fields.functionality_state_.size();  ++i) {
    if ('1' == fields.functionality_state_[i]) {
      this->func_states_[i] = 1;
    }
  }

  // 设置VIP
  this->vip_expire_time_ = now_time;
  if(now_time - this->GetLastLogoutTime() >= fields.vip_remainder_time_) {
    this->AlterVip(entity::VipType::INVALID, 0, false);
  } else {
    this->AlterVip((entity::VipType::type)this->GetAttribute(entity::ActorAoiFields::VIP), 
        fields.vip_remainder_time_ - (now_time - this->GetLastLogoutTime()), false);
  }

  // 设置防沉迷
  this->SetAttribute(entity::ActorClientFields::FCM_TYPE,
      fields.fcm_type_, false);
  this->fcm_online_time_ = fields.fcm_online_time_;
  if(this->InitializeFcm(now_time) == false) {
    LOG_ERROR("初始化防沉迷失败");
    return false;
  }

  // 设置护符值
  this->current_hp_talisman_value_ = fields.hp_tasliman_;
  this->current_mp_talisman_value_ = fields.mp_tasliman_;

  // 更新角色基础属性战斗力
  this->UpdateActorBaseFightScore(false);

  // 注册一个定时器，用于定时保存数据
  GameServerSingleton::GetInstance().AddTimer(TIMER_ID_SAVE,
      global::ServerConfigureSingleton::GetInstance().GetAutoSave().actor_ * 1000,
      -1, this, "GameActor::Initialize");

  // 注册一个定时器，用于定时检测名字颜色
  GameServerSingleton::GetInstance().AddTimer(TIMER_NAME_COLOR,
      (core::uint32)(MISC_CONF()->brown_time_ * 0.5 * 1000),
      -1, this, "GameActor::Initialize");


  // 设置GM扩展加载结束
  this->GetLoadProgress().LoadExtensionFinish(ExtensionType::GM);

  // 设置护符
  this->last_hp_talisman_use_time_ = 0;
  this->last_mp_talisman_use_time_ = 0;

  return true;
}

void GameActor::Finalize() {
  GameRole::Finalize();
  this->load_progress_.Finalize();
  // 删除数据保存定时器
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_ID_SAVE, this);
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_NAME_COLOR, this);
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_VIP_TIME, this);
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_FCM_HALF_INCOME, this);
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_FCM_ZERO_INCOME, this);
}

void GameActor::RequireLogout() {
  this->SetLastLogoutTime(GlobalTimeTick::GetInstance()->TimeTick().GetRealSec());
  this->Save();
  this->Offline();
}

void GameActor::OnExtentionLoadFinish() {
  this->attribute_update_.UpdateAll();
  this->actor_attribute_update_.UpdateAll();
  if(this->load_current_hp_ > 0) {
    this->SetAttribute(entity::RoleAoiFields::CURRENT_HP, this->load_current_hp_, false);
  } else {
    this->SetBasicStatus(entity::RoleBasicStatus::STATUS_DIE, false);
  }
  if(this->load_current_mp_ > 0) {
    this->SetAttribute(entity::RoleAoiFields::CURRENT_MP, this->load_current_mp_, false);
  }
  this->Synchronize();
  this->OpenFunctionality(this->GetAttribute(entity::RoleAoiFields::LEVEL),
      this->GetAttribute(entity::RoleAoiFields::LEVEL));
}

void GameActor::Offline() {
  global::LogDebug("[玩家下线流程] 玩家(%lld, %s)从游戏服务器下线",
      this->GetActorID(), this->GetName().c_str());

  // 发送登出事件
  event::EventActorLogout message;
  message.__set_id_(this->GetID());

  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_LOGOUT, this->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &message,
      sizeof(event::EventActorLogout));
}

void GameActor::Save() {
  // 发送保存事件
  event::EventActorSave message;
  message.__set_id_(this->GetID());
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_SAVE, this->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &message, sizeof(message));

  // TODO: 优化存储效率，设置一个 save 标识，如果有需要存储的属性改变，则设置这个标识，
  // 在这里判断这个标识是否被设置，如果设置则保存，如果没有设置，直接返回。
  // 注意：保存事件的发送和标识没有关系。

  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  // 填充保存消息
  database::protocol::StorageActorSaveRequest request;
  request.__set_id_(this->GetID());
  // 保存下线时间防止服务器崩溃导致跨天问题
  request.fields_.__set_last_logout_time_(now);
  request.fields_.__set_create_time_(this->create_time_);
  request.fields_.__set_level_(this->GetAttribute(entity::RoleAoiFields::LEVEL));
  request.fields_.__set_vocation_(this->GetAttribute(entity::RoleAoiFields::VOCATION));
  request.fields_.__set_gender_(this->GetAttribute(entity::ActorAoiFields::GENDER));
  core::uint32 appearance = this->GetAttribute(entity::ActorAoiFields::APPEARANCE);
  request.fields_.__set_head_(appearance & 0xFF);
  request.fields_.__set_hair_((appearance & 0xFF00) >> 8);
  request.fields_.__set_current_hp_(this->GetAttribute(
        entity::RoleAoiFields::CURRENT_HP));
  request.fields_.__set_current_mp_(this->GetAttribute(
        entity::RoleAoiFields::CURRENT_MP));
  request.fields_.__set_current_exp_(this->GetAttribute(
        entity::ActorClientFields::CURRENT_EXP));
  request.fields_.__set_nimbus_(this->GetAttribute(
        entity::ActorClientFields::NIMBUS));
  request.fields_.__set_gold_(this->GetAttribute(
        entity::ActorClientFields::GOLD));
  request.fields_.__set_freedom_dollars_(this->GetAttribute(
        entity::ActorClientFields::FREEDOM_DOLLARS));
  request.fields_.__set_restrict_dollars_(this->GetAttribute(
        entity::ActorClientFields::RESTRICT_DOLLARS));
  request.fields_.__set_honour_(this->GetAttribute(
        entity::ActorClientFields::HONOUR));
  request.fields_.__set_war_soul_(this->GetAttribute(
        entity::ActorClientFields::WAR_SOUL));
  request.fields_.__set_gem_shard_(this->GetAttribute(
        entity::ActorClientFields::GEM_SHARD));
  request.fields_.__set_justice_(this->GetAttribute(
        entity::ActorClientFields::JUSTICE));
  request.fields_.__set_miracle_integral_(this->GetAttribute(
        entity::ActorClientFields::MIRACLE_INTEGRAL));
  request.fields_.__set_arena_integral_(this->GetAttribute(
        entity::ActorClientFields::ARENA_INTEGRAL));
  request.fields_.__set_legend_integral_(this->GetAttribute(
        entity::ActorClientFields::LEGEND_INTEGRAL));
  request.fields_.__set_map_(global::GameMisc::GetSceneMapID(this->GetScene()));
  this->GetPosition((core::uint32 &)request.fields_.x_,
      (core::uint32 &)request.fields_.y_);
  request.fields_.__set_pre_map_(this->GetPreMap());
  request.fields_.__set_pre_x_(this->GetPrePosx());
  request.fields_.__set_pre_y_(this->GetPrePosy());
  request.fields_.__set_brown_name_(this->GetAttribute(entity::ActorAoiFields::BROWN_NAME));
  request.fields_.__set_pk_mode_(this->GetAttribute(entity::ActorClientFields::PK_MODE));
  request.fields_.__set_packet_capacity_(this->GetAttribute(
        entity::ActorClientFields::PACKET_CAPACITY));
  request.fields_.__set_storage_capacity_(this->GetAttribute(
        entity::ActorClientFields::STORAGE_CAPACITY));
  request.fields_.__set_axe_fight_score_(this->GetFightScore(entity::FightScoreType::AXE));
  request.fields_.__set_shoot_fight_score_(this->GetFightScore(entity::FightScoreType::SHOOT));
  request.fields_.__set_magic_fight_score_(this->GetFightScore(entity::FightScoreType::MAGIC));
  request.fields_.__set_world_jump_vip_num_(this->GetWorldJumpVipNum());
  request.fields_.__set_fcm_type_(this->GetAttribute(
        entity::ActorClientFields::FCM_TYPE));
  request.fields_.__set_fcm_online_time_(this->fcm_online_time_ + now - this->login_time_);

  // 系统功能开启状态
  request.fields_.functionality_state_.resize(this->func_states_.size(), '0');
  for (size_t i = 0; i < this->func_states_.size(); ++i) {
    if (this->func_states_[i] != 0) {
      request.fields_.functionality_state_[i] = '1';
    }
  }

  // 保存VIP信息
  if(now >= this->vip_expire_time_) {
    request.fields_.__set_vip_type_(entity::VipType::INVALID);
    request.fields_.__set_vip_remainder_time_(0);
  } else {
    request.fields_.__set_vip_type_(this->GetAttribute(entity::ActorAoiFields::VIP));
    request.fields_.__set_vip_remainder_time_(this->vip_expire_time_ - now);
  }

  // 保存护符值
  request.fields_.__set_hp_tasliman_(this->current_hp_talisman_value_);
  request.fields_.__set_mp_tasliman_(this->current_mp_talisman_value_);

  // 请求保存
  if(database::client::DatabaseClientSingleton::GetInstance().SendRequest(
        request, database::protocol::StorageType::STORAGE_ACTOR_SAVE,
        this->GetID()) == false) {
    global::LogError("%s:%d (%s) 向数据库代理服务器请求失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  global::LogInfo("[玩家数据保存] 玩家(%lu, %s) 进行数据保存",
      this->GetActorID(), this->GetName().c_str());
}

void GameActor::CheckNameColor() {
  // 如果是褐名开始计数
  core::int32 name_color_value = this->GetAttribute(entity::ActorAoiFields::BROWN_NAME);
  if(name_color_value > 0) {
   this->SetAttribute(entity::ActorAoiFields::BROWN_NAME, name_color_value - 1);
  }
}

core::int32 GameActor::GetAttribute(entity::ActorAoiFields::type type) const {
  if(type < entity::ActorAoiFields::AOI_END) {
    return this->aoi_attribute_[type];
  }
  return 0;
}

bool GameActor::SetAttribute(entity::RoleAoiFields::type type,
    core::int32 value, bool synchronize) {
  if (type == entity::RoleAoiFields::VOCATION && synchronize) {
    entity::VocationType::type src_vocation =
      (entity::VocationType::type)this->GetAttribute(entity::RoleAoiFields::VOCATION);

    if (GameRole::SetAttribute(type, value, synchronize) == false) {
      return false;
    }

    // 同步到 session_server
    session::protocol::MessageActorVocationSynchronzie sync;
    sync.__set_actor_id_(this->GetID());
    sync.__set_src_vocation_(src_vocation);
    sync.__set_dst_vocation_(
        (entity::VocationType::type)this->GetAttribute(entity::RoleAoiFields::VOCATION));
    SessionChannel::SendMessage(sync,
        session::protocol::GameMessageType::MESSAGE_ACTOR_VOCATION_SYNCHRONIZE);

    return true;

  } else {
    return GameRole::SetAttribute(type, value, synchronize);
  }
}

bool GameActor::SetAttribute(entity::ActorAoiFields::type type,
    core::int32 value, bool synchronize) {
  if(type < entity::ActorAoiFields::AOI_END) {
    // 设置属性
    this->aoi_attribute_[type] = value;
    // 修正属性
    this->ReviseAttribute(type);
    // 更新相关属性
    this->actor_attribute_update_.Update(type, synchronize);
    if(synchronize) {
      this->Synchronize(type);
    }
    return true;
  }
  return false;
}

core::int32 GameActor::GetAttribute(entity::ActorClientFields::type type) const {
  if(type < entity::ActorClientFields::CLIENT_END) {
    return this->client_attribute_[type];
  }
  return 0;
}

bool GameActor::SetAttribute(entity::ActorClientFields::type type,
    core::int32 value, bool synchronize) {
  if (type >= entity::ActorClientFields::CLIENT_END) {
    return false;
  }

  if (entity::ActorClientFields::CURRENT_EXP == type) {
    // 经验
    core::int32 old_level = this->GetAttribute(entity::RoleAoiFields::LEVEL);
    core::int32 level = old_level;
    core::int32 exp = value;

    const AccordingLevelCell *cell =
      Configure::GetInstance()->GetAccordingLevelConfigure().GetCell(level);
    if (NULL == cell) {
      LOG_ERROR("AccordingLevelCell(%lu) not found in config.", level);
      return false;
    }

    while (exp >= cell->exp_) {
      const AccordingLevelCell *next_cell =
        Configure::GetInstance()->GetAccordingLevelConfigure().GetCell(level + 1);
      if (NULL == next_cell) {
        exp = cell->exp_ - 1;
        break;
      }
      ++level;
      exp -= cell->exp_;
      cell = next_cell;
    }
    this->client_attribute_[type] = exp;
    this->ReviseAttribute(type);
    if (synchronize) {
      this->Synchronize(type);
    }

    if (level != old_level) {
      // 设置等级
      this->SetAttribute(entity::RoleAoiFields::LEVEL, level, synchronize);
      // 升级时生命值、魔法值加满
      this->SetAttribute(entity::RoleAoiFields::CURRENT_HP, this->GetAttribute(entity::RoleAoiFields::MAX_HP));
      this->SetAttribute(entity::RoleAoiFields::CURRENT_MP, this->GetAttribute(entity::RoleAoiFields::MAX_MP));
      // 发送升级事件
      event::EventActorLevelUp event;
      event.__set_actor_id_(this->GetID());
      event.__set_old_level_(old_level);
      event.__set_new_level_(level);
      coresh::CommunicatorSingleton::GetInstance().Broadcast(
          event::EventType::EVENT_ACTOR_LEVEL_UP, this->GetID(),
          event::ChannelType::CHANNEL_ACTOR, &event,
          sizeof(event::EventActorLevelUp));
      // 同步 session_server
      session::protocol::MessageActorLevelSynchronize sync;
      sync.__set_actor_id_(this->GetID());
      sync.__set_old_level_(old_level);
      sync.__set_new_level_(level);
      SessionChannel::SendMessage(sync,
          session::protocol::GameMessageType::MESSAGE_ACTOR_LEVEL_SYNCHRONIZE);
      // 开启功能
      this->OpenFunctionality(old_level, level);
      // 更新角色基础属性战斗力
      this->UpdateActorBaseFightScore();
    }
  } else {
    if(type == entity::ActorClientFields::NIMBUS) {
      if (value < 0) {
        value = 0;
      } else if (value > MISC_CONF()->max_nimbus_) {
        value = MISC_CONF()->max_nimbus_;
      }
    }
    // 设置属性
    this->client_attribute_[type] = value;
    // 修正属性
    this->ReviseAttribute(type);
    // 更新相关属性
    this->actor_attribute_update_.Update(type, synchronize);
    if(synchronize) {
      this->Synchronize(type);
    }
  }

  return true;
}

core::int32 GameActor::GetAttribute(entity::ActorServerFields::type type) const {
  if(type < entity::ActorServerFields::SERVER_END) {
    return this->server_attribute_[type];
  }
  return 0;
}

bool GameActor::SetAttribute(entity::ActorServerFields::type type,
    core::int32 value, bool synchronize) {
  if(type < entity::ActorServerFields::SERVER_END) {
    // 设置属性
    this->server_attribute_[type] = value;
    // 修正属性
    this->ReviseAttribute(type);
    // 更新相关属性
    this->actor_attribute_update_.Update(type, synchronize);
  }
  return true;
}

void GameActor::AddExp(core::int32 add_value) {
  // 加成经验值暂时不会为负值
  if(add_value < 0) {
    LOG_ERROR("玩家[%lu]加成经验[%d]出现错误", this->GetActorID(),
        add_value);
    return ;
  }
  // 同步客户端
  gateway::protocol::MessageAddExpSynchronize message;
  message.__set_exp_(add_value);
  this->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ADD_EXP_SYNCHRONIZE);
  // 加成经验
  this->SetAttribute(entity::ActorClientFields::CURRENT_EXP, this->GetAttribute(
        entity::ActorClientFields::CURRENT_EXP) + add_value);
}

core::int32 GameActor::GetResource(entity::ResourceID::type type) const
{
  switch(type) {
    case entity::ResourceID::GOLD:
      return this->GetAttribute(entity::ActorClientFields::GOLD);
      break;
    case entity::ResourceID::FREEDOM_DOLLARS:
      return this->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS);
      break;
    case entity::ResourceID::RESTRICT_DOLLARS:
      return this->GetAttribute(entity::ActorClientFields::RESTRICT_DOLLARS);
      break;
    case entity::ResourceID::NIMBUS:
      return this->GetAttribute(entity::ActorClientFields::NIMBUS);
      break;
    case entity::ResourceID::EXP:
      return this->GetAttribute(entity::ActorClientFields::CURRENT_EXP);
      break;
    case entity::ResourceID::WAR_SOUL:
      return this->GetAttribute(entity::ActorClientFields::WAR_SOUL);
      break;
    case entity::ResourceID::GEM_SHARD:
      return this->GetAttribute(entity::ActorClientFields::GEM_SHARD);
      break;
    case entity::ResourceID::HONOUR:
      return this->GetAttribute(entity::ActorClientFields::HONOUR);
      break;
    case entity::ResourceID::JUSTICE:
      return this->GetAttribute(entity::ActorClientFields::JUSTICE);
      break;
    case entity::ResourceID::MIRACLE_INTEGRAL:
      return this->GetAttribute(entity::ActorClientFields::MIRACLE_INTEGRAL);
      break;
    case entity::ResourceID::LEGEND_INTEGRAL:
      return this->GetAttribute(entity::ActorClientFields::LEGEND_INTEGRAL);
      break;
    case entity::ResourceID::ARENA_INTEGRAL:
      return this->GetAttribute(entity::ActorClientFields::ARENA_INTEGRAL);
      break;
    default:
      LOG_ERROR("资源[%d]没有被加入", type);
      break;
  }
  return 0;
}

void GameActor::AddResource(entity::ResourceID::type type, core::int32 value) {
  // 获取资源数目
  core::int32 old_num = 0;
  core::int32 new_num = 0;
  // 加入资源
  switch(type) {
    case entity::ResourceID::GOLD:
      old_num = this->GetAttribute(entity::ActorClientFields::GOLD);
      this->SetAttribute(entity::ActorClientFields::GOLD,
          this->GetAttribute(entity::ActorClientFields::GOLD) + value);
      new_num = this->GetAttribute(entity::ActorClientFields::GOLD);
      break;
    case entity::ResourceID::FREEDOM_DOLLARS:
      old_num = this->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS);
      this->SetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS,
          this->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS) + value);
      new_num = this->GetAttribute(entity::ActorClientFields::FREEDOM_DOLLARS);
      break;
    case entity::ResourceID::RESTRICT_DOLLARS:
      old_num = this->GetAttribute(entity::ActorClientFields::RESTRICT_DOLLARS);
      this->SetAttribute(entity::ActorClientFields::RESTRICT_DOLLARS,
          this->GetAttribute(entity::ActorClientFields::RESTRICT_DOLLARS) + value);
      new_num = this->GetAttribute(entity::ActorClientFields::RESTRICT_DOLLARS);
      break;
    case entity::ResourceID::NIMBUS:
      old_num = this->GetAttribute(entity::ActorClientFields::NIMBUS);
      this->SetAttribute(entity::ActorClientFields::NIMBUS,
          this->GetAttribute(entity::ActorClientFields::NIMBUS) + value);
      new_num = this->GetAttribute(entity::ActorClientFields::NIMBUS);
      break;
    case entity::ResourceID::EXP:
      old_num = this->GetAttribute(entity::ActorClientFields::CURRENT_EXP);
      this->AddExp(value);
      new_num = this->GetAttribute(entity::ActorClientFields::CURRENT_EXP);
      break;
    case entity::ResourceID::WAR_SOUL:
      old_num = this->GetAttribute(entity::ActorClientFields::WAR_SOUL);
      this->SetAttribute(entity::ActorClientFields::WAR_SOUL,
          this->GetAttribute(entity::ActorClientFields::WAR_SOUL) + value);
      new_num = this->GetAttribute(entity::ActorClientFields::WAR_SOUL);
      break;
    case entity::ResourceID::GEM_SHARD:
      old_num = this->GetAttribute(entity::ActorClientFields::GEM_SHARD);
      this->SetAttribute(entity::ActorClientFields::GEM_SHARD,
          this->GetAttribute(entity::ActorClientFields::GEM_SHARD) + value);
      new_num = this->GetAttribute(entity::ActorClientFields::GEM_SHARD);
      break;
    case entity::ResourceID::HONOUR:
      old_num = this->GetAttribute(entity::ActorClientFields::HONOUR);
      this->SetAttribute(entity::ActorClientFields::HONOUR,
          this->GetAttribute(entity::ActorClientFields::HONOUR) + value);
      new_num = this->GetAttribute(entity::ActorClientFields::HONOUR);
      break;
    case entity::ResourceID::JUSTICE:
      old_num = this->GetAttribute(entity::ActorClientFields::JUSTICE);
      this->SetAttribute(entity::ActorClientFields::JUSTICE,
          old_num + value);
      new_num = this->GetAttribute(entity::ActorClientFields::JUSTICE);
      break;
    case entity::ResourceID::MIRACLE_INTEGRAL:
      old_num = this->GetAttribute(entity::ActorClientFields::MIRACLE_INTEGRAL);
      this->SetAttribute(entity::ActorClientFields::MIRACLE_INTEGRAL,
          old_num + value);
      new_num = this->GetAttribute(entity::ActorClientFields::MIRACLE_INTEGRAL);
      break;
    case entity::ResourceID::LEGEND_INTEGRAL:
      old_num = this->GetAttribute(entity::ActorClientFields::LEGEND_INTEGRAL);
      this->SetAttribute(entity::ActorClientFields::LEGEND_INTEGRAL,
          old_num + value);
      new_num = this->GetAttribute(entity::ActorClientFields::LEGEND_INTEGRAL);
      break;
    case entity::ResourceID::ARENA_INTEGRAL:
      old_num = this->GetAttribute(entity::ActorClientFields::ARENA_INTEGRAL);
      this->SetAttribute(entity::ActorClientFields::ARENA_INTEGRAL,
          old_num + value);
      new_num = this->GetAttribute(entity::ActorClientFields::ARENA_INTEGRAL);
      break;
    case entity::ResourceID::GUILD_CONTRIBUTION:
      // 转发 session_server
      {
        session::protocol::MessageGuildAddContributionRequest request;
        request.__set_actor_id_(this->GetID());
        request.__set_add_value_(value);
        SessionChannel::SendMessage(request,
            session::protocol::GameMessageType::MESSAGE_GUILD_ADD_CONTRIBUTION_REQUEST);
      }
      break;
    default:
      LOG_ERROR("资源[%d]没有被加入[%d]", type, value);
      break;
  }
  event::EventActorResourceChange event;
  event.__set_actor_(this->GetActorID());
  event.__set_type_(type);
  event.__set_old_num_(old_num);
  event.__set_new_num_(new_num);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_ACTOR_RESOURCE_CHANGE, this->GetActorID(), event::ChannelType::CHANNEL_ACTOR,
      &event, sizeof(event));
}

bool GameActor::CheckFunctionalityState(entity::FunctionalityType::type type) const {
  return this->func_states_[type];
}

void GameActor::SetFunctionalityState(entity::FunctionalityType::type type,
    bool state, bool synchronize) {
  if (state == (bool)this->func_states_[type]) {
    return;
  }
  this->func_states_[type] = state;

  // 发送玩家系统功能开启事件
  if (true == state) {
    event::EventActorFunctionalityEnable event;
    event.__set_actor_id_(this->GetID());
    event.__set_functionality_id_(type);
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_ACTOR_FUNCTIONALITY_ENABLE , this->GetID(),
        event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
  }

  // 同步 session_server
  session::protocol::MessageActorFunctionalityStateSynchronize session_sync;
  session_sync.__set_actor_id_(this->GetID());
  session_sync.__set_type_(type);
  session_sync.__set_state_(state);
  SessionChannel::SendMessage(session_sync,
      session::protocol::GameMessageType::MESSAGE_ACTOR_FUNCTIONALITY_STATE_SYCHRONIZE);

  // 同步 client
  if (synchronize) {
    gateway::protocol::MessageSynchronizeActorFunctionalityState client_sync;
    client_sync.__set_id_(type);
    client_sync.__set_state_(state);
    this->SendMessage(client_sync,
        gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_ACTOR_FUNCTIONALITY_STATE);
  }
}

std::string GameActor::GetFunctionalityState() const {
  std::string functionality_state(this->func_states_.size(), '0');

  for (size_t i = 0; i < this->func_states_.size(); ++i) {
    if (this->func_states_[i] != 0) {
      functionality_state[i] = '1';
    }
  }

  return functionality_state;
}

core::int32 GameActor::GetSubFightScore(entity::SubFightScoreType::type type) const {
  return this->sub_fight_scores_[type];
}

void GameActor::SetSubFightScore(entity::SubFightScoreType::type type, core::int32 score,
    bool synchronize) {
  this->sub_fight_scores_[type] = score;

  if (synchronize) {
    // 同步 session_server
    session::protocol::MessageActorFightScoreSychronize session_sync;
    session_sync.__set_actor_id_(this->GetID());

    if (type >= entity::SubFightScoreType::MIN &&
        type < entity::SubFightScoreType::AXE_EQUIPMENT) {
      session_sync.__set_axe_fight_score_(this->GetFightScore(entity::FightScoreType::AXE));
      session_sync.__set_shoot_fight_score_(this->GetFightScore(entity::FightScoreType::SHOOT));
      session_sync.__set_magic_fight_score_(this->GetFightScore(entity::FightScoreType::MAGIC));
    } else if (entity::SubFightScoreType::AXE_EQUIPMENT == type ||
               entity::SubFightScoreType::AXE_RUNE == type) {
      session_sync.__set_axe_fight_score_(this->GetFightScore(entity::FightScoreType::AXE));
    } else if (entity::SubFightScoreType::SHOOT_EQUIPMENT == type ||
               entity::SubFightScoreType::SHOOT_RUNE == type) {
      session_sync.__set_shoot_fight_score_(this->GetFightScore(entity::FightScoreType::SHOOT));
    } else if (entity::SubFightScoreType::MAGIC_EQUIPMENT == type ||
               entity::SubFightScoreType::MAGIC_RUNE == type) {
      session_sync.__set_magic_fight_score_(this->GetFightScore(entity::FightScoreType::MAGIC));
    }

    SessionChannel::SendMessage(session_sync,
        session::protocol::GameMessageType::MESSAGE_ACTOR_FIGHT_SCORE_SYCHRONIZE);

    // 同步 client
    gateway::protocol::MessageSynchronizeActorSubFightScore client_sync;
    client_sync.__set_id_(type);
    client_sync.__set_sub_fight_score_(score);
    this->SendMessage(client_sync,
        gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_ACTOR_SUB_FIGHT_SCORE);
  }
}

core::int32 GameActor::GetCommonPartFightScore() const {
  core::int32 score = 0;

  for (int i = entity::SubFightScoreType::MIN;
       i < entity::SubFightScoreType::AXE_EQUIPMENT; ++i) {
    score += this->sub_fight_scores_[i];
  }

  return score;
}

core::int32 GameActor::GetFightScore(entity::FightScoreType::type type) const {
  if (entity::FightScoreType::AXE == type) {
    return this->GetSubFightScore(entity::SubFightScoreType::AXE_EQUIPMENT) +
           this->GetSubFightScore(entity::SubFightScoreType::AXE_RUNE) +
           this->GetCommonPartFightScore();
  } else if (entity::FightScoreType::SHOOT == type) {
    return this->GetSubFightScore(entity::SubFightScoreType::SHOOT_EQUIPMENT) +
           this->GetSubFightScore(entity::SubFightScoreType::SHOOT_RUNE) +
           this->GetCommonPartFightScore();
  } else if (entity::FightScoreType::MAGIC == type) {
    return this->GetSubFightScore(entity::SubFightScoreType::MAGIC_EQUIPMENT) +
           this->GetSubFightScore(entity::SubFightScoreType::MAGIC_RUNE) +
           this->GetCommonPartFightScore();
  } else {
    LOG_ERROR("FightScoreType is invalid.");
    return 0;
  }
}

core::int32 GameActor::GetCurrentFightScore() const {
  if (this->GetAttribute(entity::RoleAoiFields::VOCATION) == entity::VocationType::AXE) {
    return this->GetFightScore(entity::FightScoreType::AXE);
  } else if (this->GetAttribute(entity::RoleAoiFields::VOCATION) == entity::VocationType::SHOOT) {
    return this->GetFightScore(entity::FightScoreType::SHOOT);
  } else if (this->GetAttribute(entity::RoleAoiFields::VOCATION) == entity::VocationType::MAGIC) {
    return this->GetFightScore(entity::FightScoreType::MAGIC);
  } else {
    LOG_ERROR("Vocation is invalid.");
    return 0;
  }
}

void GameActor::UpdateActorBaseFightScore(bool synchronize) {
  const VocationGrowthCell *cell = Configure::GetInstance()->GetVocationGrowthCell(
      (entity::VocationType::type)this->GetAttribute(entity::RoleAoiFields::VOCATION));
  if (NULL == cell) {
    LOG_ERROR("VocationGrowthCell(%d) not found in config.",
              this->GetAttribute(entity::RoleAoiFields::VOCATION));
    return;
  }
  core::int32 level = this->GetAttribute(entity::RoleAoiFields::LEVEL);

  FightScoreCalculator calc;
  calc.physique_ = level * cell->physique_ / 100 + Configure::kInitialActorAttr_;
  calc.strength_ = level * cell->strength_ / 100 + Configure::kInitialActorAttr_;
  calc.intelligence_ = level * cell->intelligence_ / 100 + Configure::kInitialActorAttr_;
  calc.agile_ = level * cell->agile_ / 100 + Configure::kInitialActorAttr_;
  calc.spirit_ = level * cell->spirit_ / 100 + Configure::kInitialActorAttr_;

  this->SetSubFightScore(entity::SubFightScoreType::ACTOR_BASE,
      calc.GetFightScore(), synchronize);
}

entity::EntityType::type GameActor::GetType() const {
  return entity::EntityType::TYPE_ACTOR;
}

void GameActor::Synchronize() {
  time_t now = GameServerSingleton::GetInstance().GetTimeTick().GetRealSec();
  core::int32 world_jump = 0;
  const global::configure::VipCell *vip_cell = VIP_CONF()->GetCell(
      (entity::VipType::type)this->GetAttribute(entity::ActorAoiFields::VIP),
      entity::VipPrivilegeType::SCENE_WORLD_JUMP_NUM);
  if (vip_cell != NULL && vip_cell->value1_ >= this->GetWorldJumpVipNum()) {
    world_jump = vip_cell->value1_ - this->GetWorldJumpVipNum();
  }

  // 发送属性到客户端
  gateway::protocol::MessageCreateActor message;
  message.__set_name_(this->GetName());
  message.__set_entity_client_fields_(this->GetEntityClientAttributes());
  message.__set_role_aoi_fields_(this->GetRoleAoiAttributes());
  message.__set_role_client_fields_(this->GetRoleClientAttributes());
  message.__set_actor_aoi_fields_(this->GetActorAoiAttributes());
  message.__set_actor_client_fields_(this->GetActorClientAttributes());
  message.__set_vip_remainder_time_(this->vip_expire_time_ - now);
  message.__set_world_jump_vip_num_(world_jump);
  message.__set_fcm_online_time_(this->fcm_online_time_);
  message.__set_create_time_(global::ToString(this->create_time_));
  message.__set_current_hp_tasliman_value_(this->current_hp_talisman_value_);
  message.__set_current_mp_tasliman_value_(this->current_mp_talisman_value_);

  message.functionality_state_.resize(this->func_states_.size(), '0');
  for (size_t i = 0; i < this->func_states_.size(); ++i) {
    if (this->func_states_[i] != 0) {
      message.functionality_state_[i] = '1';
    }
  }

  for (size_t i = 0; i < this->sub_fight_scores_.size(); ++i) {
    message.sub_fight_scores_.push_back(this->sub_fight_scores_[i]);
  }

  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%lu", now);
  message.system_now_ = buffer;
  
  this->SendMessage(message, gateway::protocol::MessageType::MESSAGE_CREATE_ACTOR);

  // 同步战斗力到 session_server
  session::protocol::MessageActorFightScoreSychronize session_sync;
  session_sync.__set_actor_id_(this->GetID());
  session_sync.__set_axe_fight_score_(this->GetFightScore(entity::FightScoreType::AXE));
  session_sync.__set_shoot_fight_score_(this->GetFightScore(entity::FightScoreType::SHOOT));
  session_sync.__set_magic_fight_score_(this->GetFightScore(entity::FightScoreType::MAGIC));
  SessionChannel::SendMessage(session_sync,
      session::protocol::GameMessageType::MESSAGE_ACTOR_FIGHT_SCORE_SYCHRONIZE);
}

void GameActor::Synchronize(entity::ActorAoiFields::type type) {
  // Get field.
  entity::ActorAoiField field;
  field.__set_field_(type);
  field.__set_value_(this->GetAttribute(type));
  // Message.
  gateway::protocol::MessageSynchronizeActor message;
  message.__set_actor_id_(this->GetStringID());
  message.actor_aoi_fields_.push_back(field);
  message.__isset.actor_aoi_fields_ = true;
  // 发送给自己
  this->SendMessage(message, gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_ACTOR);
  // 发送到周围
  this->SendMessageAround(message, gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_ACTOR);
}

void GameActor::Synchronize(entity::ActorClientFields::type type) {
  // Get field.
  entity::ActorClientField field;
  field.__set_field_(type);
  field.__set_value_(this->GetAttribute(type));
  // Message.
  gateway::protocol::MessageSynchronizeActor message;
  message.__set_actor_id_(this->GetStringID());
  message.actor_client_fields_.push_back(field);
  message.__isset.actor_client_fields_ = true;
  this->SendMessage(message, gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_ACTOR);
}

void GameActor::Synchronize(entity::RoleAoiFields::type type) {
  // Get field.
  entity::RoleAoiField field;
  field.__set_field_(type);
  field.__set_value_(this->GetAttribute(type));
  // Message.
  gateway::protocol::MessageSynchronizeActor message;
  message.__set_actor_id_(this->GetStringID());
  message.role_aoi_fields_.push_back(field);
  message.__isset.role_aoi_fields_ = true;
  // 发送给自己
  this->SendMessage(message, gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_ACTOR);
  // 发送到周围
  this->SendMessageAround(message, gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_ACTOR);
}

void GameActor::Synchronize(entity::RoleClientFields::type type) {
  // Get field.
  entity::RoleClientField field;
  field.__set_field_(type);
  field.__set_value_(this->GetAttribute(type));
  // Message.
  gateway::protocol::MessageSynchronizeActor message;
  message.__set_actor_id_(this->GetStringID());
  message.role_client_fields_.push_back(field);
  message.__isset.role_client_fields_ = true;
  this->SendMessage(message, gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_ACTOR);
}

void GameActor::Synchronize(entity::EntityClientFields::type type) {
  // Get field.
  entity::EntityClientField field;
  field.__set_field_(type);
  field.__set_value_(this->GetAttribute(type));
  // Message.
  gateway::protocol::MessageSynchronizeActor message;
  message.__set_actor_id_(this->GetStringID());
  message.entity_client_fields_.push_back(field);
  message.__isset.entity_client_fields_ = true;
  this->SendMessage(message, gateway::protocol::MessageType::MESSAGE_SYNCHRONIZE_ACTOR);
}

void GameActor::OnTimer(core::uint32 id) {
  if(id == TIMER_ID_SAVE) {
    this->Save();
  } else if(id == TIMER_NAME_COLOR) {
    this->CheckNameColor();  
  } else if(id == TIMER_VIP_TIME) {
    this->VipExpired();
  } else if(id == TIMER_FCM_HALF_INCOME) {
    this->SetAttribute(entity::ActorClientFields::FCM_STATUS, entity::FcmStatusType::HALF_INCOME);
    event::EventActorFcmStatusChange event;
    event.__set_actor_(this->GetActorID());
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_ACTOR_FCM_STATUS_CHANGE, this->GetActorID(), event::ChannelType::CHANNEL_ACTOR,
        &event, sizeof(event));
  } else if(id == TIMER_FCM_ZERO_INCOME) {
    this->SetAttribute(entity::ActorClientFields::FCM_STATUS, entity::FcmStatusType::ZERO_INCOME);
    event::EventActorFcmStatusChange event;
    event.__set_actor_(this->GetActorID());
    coresh::CommunicatorSingleton::GetInstance().Broadcast(
        event::EventType::EVENT_ACTOR_FCM_STATUS_CHANGE, this->GetActorID(), event::ChannelType::CHANNEL_ACTOR,
        &event, sizeof(event));
  }
}

void GameActor::ReviseAttribute(entity::ActorAoiFields::type type) {
  static AttributeReviseFunctor functions[entity::ActorAoiFields::AOI_END] = {
  };
  // 修正
  if(functions[type]) {
    functions[type](this->aoi_attribute_[type]);
  }
}

void GameActor::ReviseAttribute(entity::ActorClientFields::type type) {
  static AttributeReviseFunctor functions[entity::ActorClientFields::CLIENT_END] = {
    NonNegativeRevise,          // 当前经验
    NonNegativeRevise,          // 灵气
    NonNegativeRevise,          // 金币
    NonNegativeRevise,          // 非绑币
    NonNegativeRevise,          // 绑定币
    NonNegativeRevise,          // 荣誉
    NonNegativeRevise,          // 战魂(神威)
    NonNegativeRevise,          // 宝石碎片
    NonNegativeRevise,          // 包裹容量
    NonNegativeRevise,          // 仓库容量
    NonNegativeRevise,          // PK模式
    NonNegativeRevise,          // 防沉迷类型
    NonNegativeRevise,          // 防沉迷状态
    NonNegativeRevise,          // 正义点
  };
  // 修正
  if(functions[type]) {
    functions[type](this->client_attribute_[type]);
  }
}

void GameActor::ReviseAttribute(entity::ActorServerFields::type type) {
  static AttributeReviseFunctor functions[entity::ActorServerFields::SERVER_END] = {
    NULL,                       // 打怪和冥想经验加成比
    NULL,                       // 打怪和冥想灵气加成比
  };
  // 修正
  if(functions[type]) {
    functions[type](this->server_attribute_[type]);
  }
}

void GameActor::OpenFunctionality(core::int32 src_level, core::int32 dest_level) {
  LoadFunctionalityConfigure::OpenCells& cells = Configure::GetInstance()->GetFunctionalityCell(
      entity::FunctionalityOpenType::ACTOR_LEVEL);
  for(core::uint32 i = 0; i < cells.size(); ++i) {
    if(cells[i].param_ >= src_level && cells[i].param_ <= dest_level && 
        this->CheckFunctionalityState(cells[i].function_) == false) {
      this->SetFunctionalityState(cells[i].function_, true, true);
    }
  }
}

bool GameActor::AlterVip(entity::VipType::type type, core::int32 vip_time, bool sync) {
  if(type > entity::VipType::MAX || type < entity::VipType::INVALID) {
    LOG_ERROR("VIP 等级参数不正确(%d)", type);
    return false;
  }

  core::int32 old_vip = this->GetAttribute(entity::ActorAoiFields::VIP);
  time_t now_time = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();

  // 按照规则
  if(type == entity::VipType::INVALID) {
    this->SetAttribute(entity::ActorAoiFields::VIP, entity::VipType::INVALID, sync);
    this->vip_expire_time_ = 0;
  } else {
    if((core::int32)type <= this->GetAttribute(entity::ActorAoiFields::VIP)) {
      this->vip_expire_time_ += vip_time;
      this->SetAttribute(entity::ActorAoiFields::VIP,
          this->GetAttribute(entity::ActorAoiFields::VIP), sync);
    } else {
      core::int32 temp_remain_time = this->vip_expire_time_ == 0 ? 0 : (this->vip_expire_time_ - now_time);
      this->vip_expire_time_ = vip_time + now_time + temp_remain_time;
      this->SetAttribute(entity::ActorAoiFields::VIP, (core::int32)type, sync);
    }
  }

  if(this->vip_expire_time_ <= now_time) {
    this->SetAttribute(entity::ActorAoiFields::VIP, entity::VipType::INVALID, sync);
    this->vip_expire_time_ = 0;
  }

  // 删除定时器
  GameServerSingleton::GetInstance().RemoveTimer(TIMER_VIP_TIME, this);
  // 安装定时器
  if(this->GetAttribute(entity::ActorAoiFields::VIP) != (core::int32)entity::VipType::INVALID) {
    GameServerSingleton::GetInstance().AddTimer(TIMER_VIP_TIME, (core::uint32)((this->vip_expire_time_ - now_time)*1000),
        1, this, "GameActor::AddVip");
  }
  if(sync) {
    this->SynchronizeVip();
    // 广播VIP变化事件
    if(old_vip != this->GetAttribute(entity::ActorAoiFields::VIP)) {
      event::EventActorVipChange event;
      event.__set_actor_(this->GetActorID());
      event.__set_old_type_((entity::VipType::type)old_vip);
      event.__set_new_type_((entity::VipType::type)this->GetAttribute(entity::ActorAoiFields::VIP));
      coresh::CommunicatorSingleton::GetInstance().Broadcast(
          event::EventType::EVENT_ACTOR_VIP_CHANGE, this->GetActorID(),
          event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));
    }
  }
  // 同步session
  session::protocol::MessageActorVipSynchronize send;
  send.__set_actor_(this->GetActorID());
  send.__set_vip_type_((entity::VipType::type)this->GetAttribute(entity::ActorAoiFields::VIP));
  send.__set_viP_expire_time_(this->vip_expire_time_);
  SessionChannel::SendMessage(send, 
      session::protocol::GameMessageType::MESSAGE_ACTOR_VIP_SYNCHRONIZE);

  return true;
}

void GameActor::VipExpired() {
  this->AlterVip(entity::VipType::INVALID, 0, true); 
}

void GameActor::SynchronizeVip() {
  gateway::protocol::MessageActorVipSynchronize message;
  time_t now = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  if(this->vip_expire_time_ <= 0) {
    message.__set_remainder_time_(0);
  } else {
    message.__set_remainder_time_((core::int32)(this->vip_expire_time_- now));
  }
  this->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ACTOR_VIP_SYNCHRONIZE);
}

bool GameActor::InitializeFcm(time_t now) {
  if(this->GetAttribute(entity::ActorClientFields::FCM_TYPE) == 
     entity::FcmType::ADULT) {
    this->fcm_online_time_ = 0;
    return true;
  }

  if(now - this->last_logout_time_ >=
     MISC_CONF()->fcm_clear_status_offline_time_) {
    this->fcm_online_time_ = 0;
  }

  if(this->fcm_online_time_ < MISC_CONF()->fcm_half_income_online_time_) {
    // 设置为正常状态
    this->SetAttribute(entity::ActorClientFields::FCM_STATUS,
        entity::FcmStatusType::NORMAL, false);
    // 设置收益减半定时器
    GameServerSingleton::GetInstance().AddTimer(TIMER_FCM_HALF_INCOME,
        (MISC_CONF()->fcm_half_income_online_time_ - this->fcm_online_time_) * 1000,
        1, this, "GameActor::InitializeFcm");
    // 设置零收益定时器
    GameServerSingleton::GetInstance().AddTimer(TIMER_FCM_ZERO_INCOME,
        (MISC_CONF()->fcm_zero_income_online_time_ - this->fcm_online_time_) * 1000,
        1, this, "GameActor::InitializeFcm");

  } else if(this->fcm_online_time_ < MISC_CONF()->fcm_zero_income_online_time_) {
    // 设置为收益减半状态
    this->SetAttribute(entity::ActorClientFields::FCM_STATUS,
        entity::FcmStatusType::HALF_INCOME, false);
    // 设置零收益定时器
    GameServerSingleton::GetInstance().AddTimer(TIMER_FCM_ZERO_INCOME,
        (MISC_CONF()->fcm_zero_income_online_time_ - this->fcm_online_time_) * 1000,
        1, this, "GameActor::InitializeFcm");

  } else {
    // 设置为零收益状态
    this->SetAttribute(entity::ActorClientFields::FCM_STATUS,
        entity::FcmStatusType::ZERO_INCOME, false);
  }

  return true;
}

void GameActor::DailyClean(core::int32 days) {
  // 清理世界跳转
  this->SetWorldJumpVipNum(0);

  core::int32 world_jump_num = 0;
  const global::configure::VipCell *vip_cell = VIP_CONF()->GetCell(
      (entity::VipType::type)this->GetAttribute(entity::ActorAoiFields::VIP),
      entity::VipPrivilegeType::SCENE_WORLD_JUMP_NUM);
  if (vip_cell != NULL) {
    world_jump_num = vip_cell->value1_;
  }

  gateway::protocol::MessageWorldJumpVipNumSynchronize message;
  message.__set_remainder_num_(world_jump_num);
  this->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_WORLD_JUMP_VIP_NUM_SYNCHRONIZE);
}

bool GameActor::UseHpTasliman() {
  // 获取当前时间
  time_t now_time = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  if(this->last_hp_talisman_use_time_ != 0) {
    if(now_time - this->last_hp_talisman_use_time_ < MISC_CONF()->hp_talisman_auto_span_time_) {
      LOG_ERROR("护符时间限制，无法使用生命护符");
      return false;
    }
  }
  // 检查血量
  if(this->GetAttribute(entity::RoleAoiFields::CURRENT_HP)/this->GetAttribute(
        entity::RoleAoiFields::MAX_HP) > MISC_CONF()->hp_under_percent_/100) {
    LOG_ERROR("护符受到玩家当前血量比限制，无法使用护符");
    return false;
  }
  core::int32 need_hp = this->GetAttribute(entity::RoleAoiFields::MAX_HP) - 
    this->GetAttribute(entity::RoleAoiFields::CURRENT_HP);
  if(need_hp <= 0) {
    LOG_ERROR("玩家血量不能加血(%d)", need_hp);
    return false;
  }
  if(this->current_hp_talisman_value_ <= 0) {
    return false;
  }
  need_hp = this->current_hp_talisman_value_ >= need_hp ? need_hp : this->current_hp_talisman_value_;
  this->SetAttribute(entity::RoleAoiFields::CURRENT_HP,this->GetAttribute(entity::RoleAoiFields::CURRENT_HP) +
      need_hp);
  this->AddHpTasliman(-need_hp);
  this->last_hp_talisman_use_time_ = now_time;
  return true;
}

bool GameActor::UseMpTasliman() {
  // 获取当前时间
  time_t now_time = GlobalTimeTick::GetInstance()->TimeTick().GetRealSec();
  if(this->last_mp_talisman_use_time_ != 0) {
    if(now_time - this->last_mp_talisman_use_time_ < MISC_CONF()->mp_talisman_auto_span_time_) {
      LOG_ERROR("护符时间限制，无法使用蓝护符");
      return false;
    }
  }
  // 检查蓝量
  if(this->GetAttribute(entity::RoleAoiFields::CURRENT_MP)/this->GetAttribute(
        entity::RoleAoiFields::MAX_MP) > MISC_CONF()->mp_under_percent_/100) {
    LOG_ERROR("护符受到玩家当前蓝量比限制，无法使用护符");
    return false;
  }
  core::int32 need_mp = this->GetAttribute(entity::RoleAoiFields::MAX_MP) - 
    this->GetAttribute(entity::RoleAoiFields::CURRENT_MP);
  if(need_mp < 0) {
    LOG_ERROR("玩家蓝量不能加蓝(%d)", need_mp);
    return false;
  }
  if(this->current_mp_talisman_value_ <= 0) {
    return false;
  }
  need_mp = this->current_mp_talisman_value_ >= need_mp ? need_mp : this->current_mp_talisman_value_;
  this->SetAttribute(entity::RoleAoiFields::CURRENT_MP,this->GetAttribute(entity::RoleAoiFields::CURRENT_MP) +
      need_mp);

  this->AddMpTasliman(-need_mp);
  this->last_mp_talisman_use_time_ = now_time;

  return true;
}

bool GameActor::AddHpTasliman(core::int32 value) {
  if(value == 0) {
    return true;
  }
  core::int32 add = this->current_hp_talisman_value_ + value;
  if(add < 0) {
    return false;
  }
  if(add > MISC_CONF()->hp_talisman_max_value_) {
    add = MISC_CONF()->hp_talisman_max_value_;
  }
  this->current_hp_talisman_value_ = add;

  gateway::protocol::MessageActorTaslimanSynchronize message;
  message.__set_type_(gateway::protocol::UseTaslimanType::HP);
  message.__set_value_(this->current_hp_talisman_value_);
  this->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ACTOR_TASLIMAN_SYNCHRONIZE);

  return true;
}

bool GameActor::AddMpTasliman(core::int32 value) {
  if(value == 0) {
    return true;
  }
  core::int32 add = this->current_mp_talisman_value_ + value;
  if(add < 0) {
    return false;
  }
  if(add > MISC_CONF()->mp_talisman_max_value_) {
    add = MISC_CONF()->mp_talisman_max_value_;
  }
  this->current_mp_talisman_value_ = add;

  gateway::protocol::MessageActorTaslimanSynchronize message;
  message.__set_type_(gateway::protocol::UseTaslimanType::MP);
  message.__set_value_(this->current_mp_talisman_value_);
  this->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_ACTOR_TASLIMAN_SYNCHRONIZE);

  return true;
}

}  // namespace server

}  // namespace game


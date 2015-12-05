#include "game_server/server/extension/guild/session_guild_message_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/global_packet.h"
#include "global/configure/configure.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_guild_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_guild_protocol_types.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/guild/facade_request.h"
#include "game_server/server/extension/guild/guild.h"
#include "game_server/server/extension/guild/guild_manager.h"
#include "game_server/server/extension/guild/guild_pool.h"
#include "game_server/server/extension/guild/guild_actor.h"
#include "game_server/server/extension/guild/guild_actor_manager.h"
#include "game_server/server/extension/guild/guild_actor_pool.h"

namespace game {

namespace server {

namespace guild {

SessionGuildMessageHandler::SessionGuildMessageHandler() {}
SessionGuildMessageHandler::~SessionGuildMessageHandler() {}

bool SessionGuildMessageHandler::Initialize() {
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_INFO_RESPONSE,
      boost::bind(&SessionGuildMessageHandler::OnMessageGuildInfoResponse, this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_INFO_SYNCHRONIZE,
      boost::bind(&SessionGuildMessageHandler::OnMessageGuildInfoSynchronize, this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_ACTOR_INITIALIZE,
      boost::bind(&SessionGuildMessageHandler::OnMessageGuildActorInitialize, this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_ACTOR_SYNCHRONIZE,
      boost::bind(&SessionGuildMessageHandler::OnMessageGuildActorSynchronize, this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_SKILL_SYNCHRONIZE,
      boost::bind(&SessionGuildMessageHandler::OnMessageGuildSkillSynchronize, this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_BUFF_SYNCHRONIZE,
      boost::bind(&SessionGuildMessageHandler::OnMessageGuildBuffSynchronize, this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_SHOP_BUY_ITEM_RESPONSE,
      boost::bind(&SessionGuildMessageHandler::OnMessageGuildShopBuyItemResponse, this, _1, _2));
  ExtensionManager::GetInstance()->SetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_PLAYING_AWARD_RESPONSE,
      boost::bind(&SessionGuildMessageHandler::OnMessageGuildPlayingAwardResponse, this, _1, _2));

  return true;
}

void SessionGuildMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_INFO_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_INFO_SYNCHRONIZE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_ACTOR_INITIALIZE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_ACTOR_SYNCHRONIZE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_SKILL_SYNCHRONIZE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_BUFF_SYNCHRONIZE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_SHOP_BUY_ITEM_RESPONSE);
  ExtensionManager::GetInstance()->ResetMessageHandler(
      session::protocol::GameMessageType::MESSAGE_GUILD_PLAYING_AWARD_RESPONSE);
}

static void SendMessageGuildShopBuyItemResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildShopBuyItemResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_SHOP_BUY_ITEM_RESPONSE);
}

static void SendMessageGuildPlayingAwardResponseFailed(GuildActor *guild_actor) {
  gateway::protocol::MessageGuildPlayingAwardResponse response;
  response.__set_result_(false);
  guild_actor->SendMessage(response,
      gateway::protocol::MessageType::MESSAGE_GUILD_PLAYING_AWARD_RESPONSE);
}

void SessionGuildMessageHandler::OnMessageGuildInfoResponse(const char *data, size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageGuildInfoResponse response;
  if (global::GlobalPacket::GetPacket()->Deserialize(response, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildInfoResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_INFO_RESPONSE from session_server, guilds_.size=[%zd].",
           response.guilds_.size());

  for (size_t i = 0; i < response.guilds_.size(); ++i) {
    session::protocol::GuildData &data = response.guilds_[i];

    // 检查公会是否存在
    Guild *guild = GuildManager::GetInstance()->GetGuild(data.id_);
    if (guild != NULL) {
      // 若存在, 更新数据
      guild->SetLevel(data.level_);
      guild->SetLightAttribute(data.light_attribute_);
      guild->SetDarkAttribute(data.dark_attribute_);
      guild->SetHolyAttribute(data.holy_attribute_);
      guild->SetShopLevel(data.shop_level_);
      guild->SetCollegeLevel(data.college_level_);
      guild->SetBarrackLevel(data.barrack_level_);

      continue;
    }

    // 若不存在, 分配新公会对象
    guild = GuildPool::GetInstance()->Allocate();
    if (NULL == guild) {
      LOG_ERROR("Allocate Guild(%lu) from GuildPool failed.", data.id_);
      return;
    }

    // 初始化
    if (guild->Initialize(data) == false) {
      GuildPool::GetInstance()->Deallocate(guild);
      LOG_ERROR("Init Guild(%lu) failed.", data.id_);
      return;
    }

    // 加入公会管理器
    if (GuildManager::GetInstance()->AddGuild(guild) == false) {
      guild->Finalize();
      GuildPool::GetInstance()->Deallocate(guild);
      LOG_ERROR("Add Guild(%lu) to GuildManager failed.", data.id_);
      return;
    }
  }
}

void SessionGuildMessageHandler::OnMessageGuildInfoSynchronize(const char *data, size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageGuildInfoSynchronize sync;
  if (global::GlobalPacket::GetPacket()->Deserialize(sync, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildInfoSynchronize failed.");
    return;
  }
  
  LOG_INFO("Receive MESSAGE_GUILD_INFO_SYNCHRONIZE from session_server, guild_id=[%lu].",
           sync.data_.id_);

  if (session::protocol::GuildOperateType::ADD == sync.type_) {
    // 新增公会对象
    Guild *guild = GuildManager::GetInstance()->GetGuild(sync.data_.id_);
    if (guild != NULL) {
      LOG_ERROR("Guild(%lu) already exists.");
      return;
    }

    // --分配公会对象
    guild = GuildPool::GetInstance()->Allocate();
    if (NULL == guild) {
      LOG_ERROR("Allocate Guild(%lu) from GuildPool failed.", sync.data_.id_);
      return;
    }

    // --初始化
    if (guild->Initialize(sync.data_) == false) {
      GuildPool::GetInstance()->Deallocate(guild);
      LOG_ERROR("Init Guild(%lu) failed.", sync.data_.id_);
      return;
    }

    // --加入公会管理器
    if (GuildManager::GetInstance()->AddGuild(guild) == false) {
      guild->Finalize();
      GuildPool::GetInstance()->Deallocate(guild);
      LOG_ERROR("Add Guild(%lu) to GuildManager failed.", sync.data_.id_);
      return;
    }

  } else if (session::protocol::GuildOperateType::REMOVE == sync.type_) {
    // 删除公会对象
    Guild *guild = GuildManager::GetInstance()->RemoveGuild(sync.data_.id_);
    if (NULL == guild) {
      LOG_ERROR("Remove Guild(%lu) from GuildManager failed.", sync.data_.id_);
      return;
    }
    guild->Finalize();
    GuildPool::GetInstance()->Deallocate(guild);

  } else if (session::protocol::GuildOperateType::UPDATE == sync.type_) {
    // 更新公会对象
    Guild *guild = GuildManager::GetInstance()->GetGuild(sync.data_.id_);
    if (NULL == guild) {
      LOG_ERROR("Get Guild(%lu) from GuildManager failed.", sync.data_.id_);
      return;
    }

    if (sync.data_.__isset.level_) {
      guild->SetLevel(sync.data_.level_);
    }
    if (sync.data_.__isset.light_attribute_) {
      guild->SetLightAttribute(sync.data_.light_attribute_);
    }
    if (sync.data_.__isset.dark_attribute_) {
      guild->SetDarkAttribute(sync.data_.dark_attribute_);
    }
    if (sync.data_.__isset.holy_attribute_) {
      guild->SetHolyAttribute(sync.data_.holy_attribute_);
    }
    if (sync.data_.__isset.shop_level_) {
      guild->SetShopLevel(sync.data_.shop_level_);
    }
    if (sync.data_.__isset.college_level_) {
      guild->SetCollegeLevel(sync.data_.college_level_);
    }
    if (sync.data_.__isset.barrack_level_) {
      guild->SetBarrackLevel(sync.data_.barrack_level_);
    }
  }

}

void SessionGuildMessageHandler::OnMessageGuildActorInitialize(const char *data, size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageGuildActorInitialize message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildActorInitialize failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_ACTOR_INITIALIZE from session_server, actor_id_=[%lu]",
           message.actor_id_);

  // 从管理器中获取玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(message.actor_id_);
  if (NULL == game_actor) {
    LOG_ERROR("Get GameActor(%lu) from GameActorManager failed.", message.actor_id_);
    return;
  }

  // 公会扩展否已存在
  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(message.actor_id_);
  if (guild_actor != NULL) {
    LOG_ERROR("GuildActor(%lu) already exists.");
    return;
  }

  // 分配公会玩家对象
  guild_actor = GuildActorPool::GetInstance()->Allocate();
  if (NULL == guild_actor) {
    LOG_ERROR("Allocate GuildActor(%lu) from GuildActorPool failed.", message.actor_id_);
    return;
  }

  // 初始化
  if (guild_actor->Initialize(game_actor, message.data_) == false) {
    GuildActorPool::GetInstance()->Deallocate(guild_actor);
    LOG_ERROR("Init GuildActor(%lu) failed.", message.actor_id_);
    return;
  }

  // 加入管理器
  if (GuildActorManager::GetInstance()->Add(guild_actor) == false) {
    guild_actor->Finalize();
    GuildActorPool::GetInstance()->Deallocate(guild_actor);
    LOG_ERROR("Add GuildActor(%lu) to GuildActorManager failed.", message.actor_id_);
    return;
  }

  // 检查 BUFF 模块是否加载完成
  if (game_actor->GetLoadProgress().CheckExtensionFinish(ExtensionType::BUFF)) {
    guild_actor->InitializeGuildBuff();
    guild_actor->SendLoadFinishEvent();
  }
}

void SessionGuildMessageHandler::OnMessageGuildActorSynchronize(const char *data, size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageGuildActorSynchronize sync;
  if (global::GlobalPacket::GetPacket()->Deserialize(sync, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildActorSynchronize failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_ACTOR_SYNCHRONIZE from session_server, actor_id_=[%lu].",
           sync.actor_id_);

  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(sync.actor_id_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", sync.actor_id_);
    return;
  }
  GameActor *game_actor = guild_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", sync.actor_id_);
    return;
  }

  if (sync.__isset.guild_id_) {
    guild_actor->SetGuildID(sync.guild_id_);
  }
  if (sync.__isset.guild_position_) {
    guild_actor->SetGuildPosition(sync.guild_position_);
  }
  if (sync.__isset.current_contribution_value_) {
    guild_actor->SetCurrentContributionValue(sync.current_contribution_value_);
  }

  // 同步场景AOI公会名属性
  if (sync.__isset.guild_name_) {
    gateway::protocol::MessageGuildAoiSynchronize client_sync;
    gateway::protocol::GuildAoiData data;
    data.__set_actor_id_(guild_actor->GetStringID());
    data.__set_guild_name_(sync.guild_name_);
    client_sync.aoi_data_.push_back(data);

    game_actor->SendMessageAround(client_sync,
        gateway::protocol::MessageType::MESSAGE_GUILD_AOI_SYNCHRONIZE, true);
  }
}

void SessionGuildMessageHandler::OnMessageGuildSkillSynchronize(const char *data, size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageGuildSkillSynchronize sync;
  if (global::GlobalPacket::GetPacket()->Deserialize(sync, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildSkillSynchronize failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_SKILL_SYNCHRONIZE from session_server, "
           "actor_id_=[%lu], type_=[%d]",
           sync.actor_id_, sync.type_);

  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(sync.actor_id_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", sync.actor_id_);
    return;
  }

  if (session::protocol::GuildSkillOperateType::ADD == sync.type_ ||
      session::protocol::GuildSkillOperateType::UPDATE == sync.type_) {
    guild_actor->SetGuildSkillLevel(sync.data_.id_, sync.data_.effective_level_);
  } else if (session::protocol::GuildSkillOperateType::CLEAR == sync.type_) {
    guild_actor->ClearGuildSkill();
  }
}

void SessionGuildMessageHandler::OnMessageGuildBuffSynchronize(const char *data, size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageGuildBuffSynchronize sync;
  if (global::GlobalPacket::GetPacket()->Deserialize(sync, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildBuffSynchronize failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_BUFF_SYNCHRONIZE from session_server, "
           "actor_id_=[%lu], type_=[%d], guild_buff_id_=[%d]",
           sync.actor_id_, sync.type_, sync.guild_buff_id_);

  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(sync.actor_id_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", sync.actor_id_);
    return;
  }

  if (session::protocol::GuildBuffOperateType::ADD == sync.type_) {
    if (guild_actor->AddGuildBuff(sync.guild_buff_id_) == false) {
      LOG_ERROR("Add GuildBuff(%d) failed.", sync.guild_buff_id_);
      return;
    }
  } else if (session::protocol::GuildBuffOperateType::CLEAR == sync.type_) {
    guild_actor->ClearGuildBuff();
  }
}

void SessionGuildMessageHandler::OnMessageGuildShopBuyItemResponse(const char *data, size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageGuildShopBuyItemResponse response;
  if (global::GlobalPacket::GetPacket()->Deserialize(response, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildShopBuyItemResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_SHOP_BUY_ITEM_RESPONSE from session_server, "
           "actor_id_=[%lu], item_id_=[%d], item_count_=[%d].",
           response.actor_id_, response.item_id_, response.item_count_);

  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(response.actor_id_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", response.actor_id_);
    return;
  }
  GameActor *game_actor = guild_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", response.actor_id_);
    return;
  }

  // 添加道具
  if (FacadeRequest::GetInstance()->AddPacketItem(response.actor_id_,
          response.item_id_, response.item_count_, entity::BindRequireType::BIND) == false) {
    LOG_ERROR("Add item failed.");
    SendMessageGuildShopBuyItemResponseFailed(guild_actor);
    return;
  }

  gateway::protocol::MessageGuildShopBuyItemResponse client_response;
  client_response.__set_result_(true);
  guild_actor->SendMessage(client_response,
      gateway::protocol::MessageType::MESSAGE_GUILD_SHOP_BUY_ITEM_RESPONSE);
}

void SessionGuildMessageHandler::OnMessageGuildPlayingAwardResponse(const char *data, size_t size) {
  if (NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 消息解析
  session::protocol::MessageGuildPlayingAwardResponse response;
  if (global::GlobalPacket::GetPacket()->Deserialize(response, data, size) == false) {
    LOG_ERROR("Deserialize MessageGuildPlayingAwardResponse failed.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GUILD_PLAYING_AWARD_RESPONSE from session_server, "
           "actor_id_=[%lu], result_=[%d], playing_template_id_=[%d].",
           response.actor_id_, response.result_, response.playing_template_id_);

  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(response.actor_id_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", response.actor_id_);
    return;
  }
  GameActor *game_actor = guild_actor->GetActor();
  if (NULL == game_actor) {
    LOG_ERROR("GameActor(%lu) is null.", response.actor_id_);
    return;
  }

  if (false == response.result_) {
    SendMessageGuildPlayingAwardResponseFailed(guild_actor);
    return;
  }

  const global::configure::GuildPlayingCell *cell =
      GUILD_CONF()->GetGuildPlaying(response.playing_template_id_);
  if (NULL == cell) {
    LOG_ERROR("playing_template_id is invalid.");
    SendMessageGuildPlayingAwardResponseFailed(guild_actor);
    return;
  }

  // 添加道具
  if (FacadeRequest::GetInstance()->AddPacketItems(
          response.actor_id_, cell->awards_) == false) {
    LOG_ERROR("Add item failed.");
    SendMessageGuildPlayingAwardResponseFailed(guild_actor);
    return;
  }

  gateway::protocol::MessageGuildPlayingAwardResponse client_response;
  client_response.__set_result_(true);
  guild_actor->SendMessage(client_response,
      gateway::protocol::MessageType::MESSAGE_GUILD_PLAYING_AWARD_RESPONSE);
}

}  // namespace guild

}  // namespace server

}  // namespace game


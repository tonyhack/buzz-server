//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-05-22 18:18:34.
// File name: gm_message_handler.cc
//
// Description:
// Define class GmMessageHandler.
//

#include "game_server/server/extension/gm/gm_message_handler.h"

#include <cstdlib>
#include <vector>
#include <string>
#include <boost/bind.hpp>

#include "entity/item_types.h"
#include "game_server/server/error_code.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/game_actor.h"
#include "game_server/server/game_actor_manager.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/request/game_ai_request_types.h"
#include "game_server/server/request/game_attr_request_types.h"
#include "game_server/server/request/game_buff_request_types.h"
#include "game_server/server/request/game_item_request_types.h"
#include "game_server/server/request/game_mail_request_types.h"
#include "game_server/server/request/game_playing_request_types.h"
#include "game_server/server/request/game_transport_request_types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "gateway_server/protocol/gateway_gm_protocol_types.h"
#include "global/common_functions.h"
#include "global/global_packet.h"
#include "global/logging.h"
#include "global/server_configure.h"
#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_gm_protocol_types.h"

namespace game {

namespace server {

namespace gm {

GmMessageHandler::GmMessageHandler() {}
GmMessageHandler::~GmMessageHandler() {}

bool GmMessageHandler::Initialize() {
  // MESSAGE_GM_ADD_ITEM
  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GM_ADD_ITEM,
      boost::bind(&GmMessageHandler::OnMessageAddItem, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GM_SCENE_JUMP,
      boost::bind(&GmMessageHandler::OnMessageSceneJump, this, _1, _2, _3));

  ExtensionManager::GetInstance()->SetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GM_COMMAND,
      boost::bind(&GmMessageHandler::OnMessageGmCommand, this, _1, _2, _3));

  return true;
}

void GmMessageHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GM_ADD_ITEM);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GM_SCENE_JUMP);

  ExtensionManager::GetInstance()->ResetMessageHandler(
      gateway::protocol::MessageType::MESSAGE_GM_COMMAND);
}

void GmMessageHandler::OnMessageAddItem(core::uint64 id,
    const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 得到玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取玩家对象(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  // 判断GM指令是否被禁用
  if (global::ServerConfigureSingleton().GetInstance().CheckGmCommandEnable() == false) {
    LOG_DEBUG("GM Command is disabled.");
    return;
  }

  // 解析消息
  gateway::protocol::MessageGmAddItem message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息 MessageGmAddItem 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  global::LogDebug("收到消息 MessageGmAddItem");

  GameActor *add_actor = GameActorManager::GetInstance()->GetActor(message.actor_name_);
  if(add_actor == NULL) {
    global::LogError("%s:%d (%s) 获取玩家对象(%s) 失败",
        __FILE__, __LINE__, __FUNCTION__, message.actor_name_.c_str());
    return ;
  }

  // 发送加物品的请求
  request::RequestAddItem request;
  request.__set_actor_id_(add_actor->GetID());
  request.__set_item_template_(message.item_template_);
  request.__set_number_(message.number_);
  request.__set_bind_(message.bind_ ?
      entity::BindRequireType::BIND : entity::BindRequireType::NOT_BIND);

  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_ITEM,
      &request, sizeof(request)) == -1) {
    global::LogError("%s:%d (%s) 请求 REQUEST_ADD_ITEM 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(request.result_ != request::AddItemResult::SUCCESS) {
    global::LogError("%s:%d (%s) 玩家(%lu) 增加道具(%u) 数量(%u) 失败",
        __FILE__, __LINE__, __FUNCTION__, add_actor->GetID(),
        request.item_template_, request.number_);
  }
}

void GmMessageHandler::OnMessageSceneJump(core::uint64 id, const char *data, size_t size) {
  if(id == 0 || data == NULL || size == 0) {
    global::LogError("%s:%d (%s) 参数错误", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  // 得到玩家对象
  GameActor *actor = GameActorManager::GetInstance()->GetActor(id);
  if(actor == NULL) {
    global::LogError("%s:%d (%s) 获取玩家对象(%lu) 失败",
        __FILE__, __LINE__, __FUNCTION__, id);
    return ;
  }

  // 判断GM指令是否被禁用
  if (global::ServerConfigureSingleton().GetInstance().CheckGmCommandEnable() == false) {
    LOG_DEBUG("GM Command is disabled.");
    return;
  }

  // 解析消息
  gateway::protocol::MessageGmSceneJump message;
  if(global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    global::LogError("%s:%d (%s) 解析消息 MessageGmSceneJump 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  global::LogDebug("玩家(%lu,%s) 使用GM指令请求跳转场景(%u)",
      actor->GetID(), actor->GetName().c_str(), message.map_);

  // 场景跳转请求
  request::RequestSceneJump request;
  request.__set_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_id_(id);
  request.__set_map_(message.map_);
  request.__set_x_(message.x_);
  request.__set_y_(message.y_);

  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SCENE_JUMP,
        &request, sizeof(request)) == -1) {
    global::LogError("%s:%d (%s) 请求 REQUEST_SCENE_JUMP 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }

  if(request.result_ == false) {
    global::LogError("%s:%d (%s) 玩家(%lu,%s) 跳转地图(%u) 失败",
        __FILE__, __LINE__, __FUNCTION__, actor->GetID(),
        actor->GetName().c_str(), message.map_);
    return ;
  }
}

///////////////////////////////////////////////////////////////////////////////

static void GmAddClientAttribute(GameActor *game_actor, entity::ActorClientFields::type attribute_id,
    std::vector<std::string> &command_args) {
  if (command_args.size() != 2) {
    LOG_WARNING("Invalid command params count.");
    return;
  }

  core::int32 increment = atoi(command_args[1].c_str());
  if (increment != 0) {
    game_actor->SetAttribute(attribute_id, game_actor->GetAttribute(attribute_id) + increment);
  }
}

static void GmEnableFuncionality(GameActor *game_actor, std::vector<std::string> &command_args) {
  if (command_args.size() != 2) {
    LOG_WARNING("Invalid command params count.");
    return;
  }

  core::int32 id = atoi(command_args[1].c_str());
  if (id < entity::FunctionalityType::MIN ||
      id >= entity::FunctionalityType::MAX) {
    LOG_WARNING("functionality_id is invalid.");
    return;
  }
  game_actor->SetFunctionalityState((entity::FunctionalityType::type)id);
}

static void GmAddSpiritFacade(GameActor *game_actor, std::vector<std::string> &command_args) {
  if (command_args.size() != 3) {
    LOG_WARNING("Invalid command params count.");
    return;
  }

  core::int32 facade_id = atoi(command_args[1].c_str());
  core::int32 duration_time = atoi(command_args[2].c_str());

  request::RequestAttrAddSpiritFacade request;
  request.__set_actor_(game_actor->GetID());
  request.__set_spirit_facade_id_(facade_id);
  request.__set_duration_time_(duration_time);
  ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ATTR_ADD_SPIRIT_FACADE,
      &request, sizeof(request));
}

static void GmAddMail(GameActor *game_actor, std::vector<std::string> &command_args) {
  if (command_args.size() != 6) {
    LOG_WARNING("Invalid command params count.");
    return ;
  }

  request::RequestMailSend request;
  request.__set_id_(game_actor->GetID());
  request.__set_child_type_((entity::SystemMailChildType::type)atoi(command_args[1].c_str()));
  request.affix_.__set_item1_(atoi(command_args[2].c_str()));
  request.affix_.__set_item1_num_(atoi(command_args[3].c_str()));
  request.affix_.__set_gold_(atoi(command_args[4].c_str()));
  request.affix_.__set_restrict_dollars_(atoi(command_args[5].c_str()));
  request.__set_type_(entity::MailType::SYSTEM);
  ExtensionManager::GetInstance()->Request(
     request::RequestType::REQUEST_MAIL_SEND, &request, sizeof(request));
}

static void GmAddItems(GameActor *game_actor, std::vector<std::string> &command_args) {
  if (command_args.size() < 3 || (command_args.size() % 2) == 0) {
    LOG_WARNING("Invalid command params count.");
    return ;
  }

  request::RequestAddItems request;

  entity::AddItemFields field;
  field.__set_bind_(false);

  for(size_t pos = 1; pos < command_args.size(); pos = pos + 2) {
    field.__set_item_template_(atoi(command_args[pos].c_str()));
    field.__set_number_(atoi(command_args[pos + 1].c_str()));
    request.items_.push_back(field);
  }

  request.__set_actor_id_(game_actor->GetID());
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_ITEMS,
        &request, sizeof(request)) == 0) {
    if(request.result_ == request::AddItemResult::PACKET_FULL) {
      ErrorCode::Send(game_actor, entity::ErrorType::PACKET_FULL);
    }
  }
}

static void GmAddBindItems(GameActor *game_actor, std::vector<std::string> &command_args) {
  if (command_args.size() < 3 || (command_args.size() % 2) == 0) {
    LOG_WARNING("Invalid command params count.");
    return ;
  }

  request::RequestAddItems request;

  entity::AddItemFields field;
  field.__set_bind_(true);

  for(size_t pos = 1; pos < command_args.size(); pos = pos + 2) {
    field.__set_item_template_(atoi(command_args[pos].c_str()));
    field.__set_number_(atoi(command_args[pos + 1].c_str()));
    request.items_.push_back(field);
  }

  request.__set_actor_id_(game_actor->GetID());
  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_ADD_ITEMS,
        &request, sizeof(request)) == 0) {
    if(request.result_ == request::AddItemResult::PACKET_FULL) {
      ErrorCode::Send(game_actor, entity::ErrorType::PACKET_FULL);
    }
  }
}

static void GmClearPlayingComplete(GameActor *game_actor, std::vector<std::string> &command_args) {
  if (command_args.size() != 2) {
    LOG_WARNING("Invalid command params count.");
    return ;
  }

  request::RequestPlayingClearComplete request;

  request.__set_actor_(game_actor->GetID());
  request.__set_playing_template_(atoi(command_args[1].c_str()));

  ExtensionManager::GetInstance()->Request(
      request::RequestType::REQUEST_PLAYING_CLEAR_COMPLETE,
      &request, sizeof(request));
}

static void GmChangeCamp(GameActor *game_actor, std::vector<std::string> &command_args) {
  if(game_actor == 0 || command_args.size() != 2) {
    LOG_ERROR("参数错误 GmChangeCamp");
    return ;
  }
  game_actor->SetAttribute(entity::RoleAoiFields::CAMP, atoi(command_args[1].c_str()));
}

static void GmAddBuff(GameActor *game_actor, std::vector<std::string> &command_args) {
  if (command_args.size() != 2) {
    LOG_WARNING("Invalid command params count.");
    return ;
  }

  request::RequestBuffStart request;
  request.__set_src_entity_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_src_entity_id_(game_actor->GetID());
  request.__set_dest_entity_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_dest_entity_id_(game_actor->GetID());
  request.__set_buff_id_(atoi(command_args[1].c_str()));

  ExtensionManager::GetInstance()->Request(
      request::RequestType::REQUEST_BUFF_START, &request, sizeof(request));
}

static void GmAddHp(GameActor *game_actor, std::vector<std::string> &command_args) {
  if (command_args.size() != 2) {
    LOG_WARNING("Invalid command params count.");
    return ;
  }

  game_actor->SetAttribute(entity::RoleAoiFields::CURRENT_HP,
      game_actor->GetAttribute(entity::RoleAoiFields::CURRENT_HP) + atoi(command_args[1].c_str()));
}

static void GmAddMp(GameActor *game_actor, std::vector<std::string> &command_args) {
  if (command_args.size() != 2) {
    LOG_WARNING("Invalid command params count.");
    return ;
  }

  game_actor->SetAttribute(entity::RoleAoiFields::CURRENT_MP,
      game_actor->GetAttribute(entity::RoleAoiFields::CURRENT_MP) + atoi(command_args[1].c_str()));
}

static void GmSummonNpc(GameActor *game_actor, std::vector<std::string> &command_args) {
  if (command_args.size() != 4) {
    LOG_WARNING("Invalid command params count.");
    return ;
  }

  request::RequestSceneSummonNpc request;
  request.__set_scene_(game_actor->GetScene());
  request.__set_template_id_(atoi(command_args[1].c_str()));
  request.__set_x_(atoi(command_args[2].c_str()));
  request.__set_y_(atoi(command_args[3].c_str()));
  request.__set_width_(0);
  request.__set_heigth_(0);

  ExtensionManager::GetInstance()->Request(
      request::RequestType::REQUEST_SCENE_SUMMON_NPC, &request, sizeof(request));
}

static void GmClearNpc(GameActor *game_actor, std::vector<std::string> &command_args) {
  const EntityKey &target = game_actor->GetSelectTarget();
  if(target.type_ != entity::EntityType::TYPE_NPC) {
    LOG_WARNING("Invalid target type.");
    return ;
  }

  request::RequestSceneClearNpc request;
  request.__set_id_(target.id_);

  ExtensionManager::GetInstance()->Request(
      request::RequestType::REQUEST_SCENE_CLEAR_NPC, &request, sizeof(request));
}

static void GmAddVip(GameActor *game_actor, std::vector<std::string>  &command_args) {
  if(command_args.size() != 3) {
    LOG_ERROR("GM GmAddVip 参数错误");
    return ;
  }
  game_actor->AlterVip((entity::VipType::type)atoi(command_args[1].c_str()),
      atoi(command_args[2].c_str()), true);
}

static void GmSetFcmType(GameActor *game_actor, std::vector<std::string>  &command_args) {
  if(command_args.size() != 2) {
    LOG_WARNING("Invalid command params count.");
    return;
  }
  core::int32 type = atoi(command_args[1].c_str());

  if(type != entity::FcmType::UNKNOWN &&
     type != entity::FcmType::MINOR &&
     type != entity::FcmType::ADULT) {
    LOG_WARNING("Invalid fcm type.");
    return;
  }
  game_actor->SetAttribute(entity::ActorClientFields::FCM_TYPE, (entity::FcmType::type)type);
}

static void GmSetFcmOnlineTime(GameActor *game_actor, std::vector<std::string>  &command_args) {
  if (command_args.size() != 2) {
    LOG_WARNING("Invalid command params count.");
    return;
  }
  core::int32 online_time = atoi(command_args[1].c_str());

  if (online_time < 0) {
    LOG_WARNING("Invalid online time.");
    return;
  }
  game_actor->SetFcmOnlineTime(online_time);
}

static void GmJump(GameActor *game_actor, std::vector<std::string> &command_args) {
  if (command_args.size() != 4) {
    LOG_WARNING("Invalid command_args count.");
    return;
  }

  // 场景跳转请求
  request::RequestSceneJump request;
  request.__set_type_(entity::EntityType::TYPE_ACTOR);
  request.__set_id_(game_actor->GetID());
  request.__set_map_(atoi(command_args[1].c_str()));
  request.__set_x_(atoi(command_args[2].c_str()));
  request.__set_y_(atoi(command_args[3].c_str()));

  if(ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_SCENE_JUMP,
        &request, sizeof(request)) == -1) {
    global::LogError("%s:%d (%s) 请求 REQUEST_SCENE_JUMP 失败",
        __FILE__, __LINE__, __FUNCTION__);
    return;
  }

  if(request.result_ == false) {
    global::LogError("%s:%d (%s) 玩家(%lu,%s) 跳转地图 失败",
        __FILE__, __LINE__, __FUNCTION__, game_actor->GetID(),
        game_actor->GetName().c_str());
    return;
  }
}

static void GmTransport(GameActor *game_actor, std::vector<std::string> &command_args) {
  if (command_args.size() != 2) {
    LOG_WARNING("Invalid command_args count.");
    return;
  }

  request::RequestTransportSummon request;
  request.__set_actor_(game_actor->GetID());
  request.__set_transport_npc_template_(atoi(command_args[1].c_str()));
  if (ExtensionManager::GetInstance()->Request(request::RequestType::REQUEST_TRANSPORT_SUMMON,
        &request, sizeof(request)) == -1 || request.transport_npc_ == 0) {
    LOG_ERROR("请求 REQUEST_TRANSPORT_SUMMON 失败");
    return;
  }
} 

static void GmAdd(GameActor *game_actor, std::vector<std::string> &command_args) {
  if(command_args.size() != 1) {
    LOG_ERROR("GM 命令错误");
    return ;
  }
  command_args.push_back(global::ToString(10000000));
  // 增加金币
  // usage: add_gold <gold_num>
  GmAddClientAttribute(game_actor, entity::ActorClientFields::GOLD, command_args);
  // 增加钻石
  // usage: add_zuanshi <freedom_dollars_num>
  GmAddClientAttribute(game_actor, entity::ActorClientFields::FREEDOM_DOLLARS, command_args);
  // 增加礼券
  // usage: add_liquan <restrict_dollars_num>
  GmAddClientAttribute(game_actor, entity::ActorClientFields::RESTRICT_DOLLARS, command_args);
  // 增加灵气
  // usage: add_lingqi <nimbus_num>
  GmAddClientAttribute(game_actor, entity::ActorClientFields::NIMBUS, command_args);
  // 增加神威
  // usage: add_shenwei <war_soul_num>
  GmAddClientAttribute(game_actor, entity::ActorClientFields::WAR_SOUL, command_args);
  // 增加经验
  // usage: add_exp <exp>
  GmAddClientAttribute(game_actor, entity::ActorClientFields::CURRENT_EXP, command_args);
  // 开启系统功能
  // usage: enable <functionality_id>
  for(size_t i = entity::FunctionalityType::MIN; i < entity::FunctionalityType::MAX; ++i) {
    command_args.pop_back();
    command_args.push_back(global::ToString(i));
    GmEnableFuncionality(game_actor, command_args);
  }

}

static void GmAddResource(GameActor *game_actor, std::vector<std::string> &command_args) {
  if(command_args.size() != 3) {
    LOG_ERROR("GM 命令错误");
    return ;
  }
  core::int32 type = atoi(command_args[1].c_str());
  core::int32 value = atoi(command_args[2].c_str());

  game_actor->AddResource((entity::ResourceID::type)type, value);
}
void GmMessageHandler::OnMessageGmCommand(core::uint64 id, const char *data, size_t size) {
  if (0 == id || NULL == data || 0 == size) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 判断GM指令是否被禁用
  if (global::ServerConfigureSingleton().GetInstance().CheckGmCommandEnable() == false) {
    LOG_DEBUG("GM Command is disabled.");
    return;
  }

  // 得到玩家对象
  GameActor *game_actor = GameActorManager::GetInstance()->GetActor(id);
  if (NULL == game_actor) {
    LOG_ERROR("Get GameActor(%lu) from GameActorManager failed.", id);
    return;
  }

  // 消息解析
  gateway::protocol::MessageGmCommand message;
  if (global::GlobalPacket::GetPacket()->Deserialize(message, data, size) == false) {
    LOG_ERROR("Deserialize MessageGmCommand falied.");
    return;
  }

  LOG_INFO("Receive MESSAGE_GM_COMMAND from actor(%lu, %s), command_=[%s].",
           game_actor->GetID(), game_actor->GetName().c_str(), message.command_.c_str());

  // 命令解析
  std::vector<std::string> command_args;
  global::StringSplit(message.command_.c_str(), " ", command_args);

  if (command_args.size() <= 0) {
    LOG_ERROR("Invalid gm command [%s].", message.command_.c_str());
    return;
  }

  if ("add_gold" == command_args[0]) {
    // 增加金币
    // usage: add_gold <gold_num>
    GmAddClientAttribute(game_actor, entity::ActorClientFields::GOLD, command_args);
  } else if ("add_zuanshi" == command_args[0]) {
    // 增加钻石
    // usage: add_zuanshi <freedom_dollars_num>
    GmAddClientAttribute(game_actor, entity::ActorClientFields::FREEDOM_DOLLARS, command_args);
  } else if ("add_liquan" == command_args[0]) {
    // 增加礼券
    // usage: add_liquan <restrict_dollars_num>
    GmAddClientAttribute(game_actor, entity::ActorClientFields::RESTRICT_DOLLARS, command_args);
  } else if ("add_lingqi" == command_args[0]) {
    // 增加灵气
    // usage: add_lingqi <nimbus_num>
    GmAddClientAttribute(game_actor, entity::ActorClientFields::NIMBUS, command_args);
  } else if ("add_shenwei" == command_args[0]) {
    // 增加神威
    // usage: add_shenwei <war_soul_num>
    GmAddClientAttribute(game_actor, entity::ActorClientFields::WAR_SOUL, command_args);
  } else if ("add_gem_shard" == command_args[0]) {
    // 增加宝石碎片
    // usage: add_gem_shard <gem_shard_num>
    GmAddClientAttribute(game_actor, entity::ActorClientFields::GEM_SHARD, command_args);
  } else if ("add_exp" == command_args[0]) {
    // 增加经验
    // usage: add_exp <exp>
    GmAddClientAttribute(game_actor, entity::ActorClientFields::CURRENT_EXP, command_args);
  } else if ("enable" == command_args[0]) {
    // 开启系统功能
    // usage: enable <functionality_id>
    GmEnableFuncionality(game_actor, command_args);
  } else if ("add_spirit_facade" == command_args[0]) {
    // 新增精灵外观
    // usage: add_spirit_facade <facade_id> <duration_time>
    GmAddSpiritFacade(game_actor, command_args);
  } else if ("add_mail" == command_args[0]) {
    // 发送系统邮件
    // usage: add_mail <context> <affix_id> <number> <gold_num> <restrict_dollars_num>
    GmAddMail(game_actor, command_args);
  } else if ("add_items" == command_args[0]) {
    // 增加道具
    // usage: add_items <item_id> <number> <item_id> <number> [...]
    GmAddItems(game_actor, command_args);
  } else if ("add_bind_items" == command_args[0]) {
    // 增加绑定道具
    // usage: add_bind_items <item_id> <number> <item_id> <number> [...]
    GmAddBindItems(game_actor, command_args);
  } else if ("clear_playing_complete" == command_args[0]) {
    // 清除副本完成次数
    // usage: clear_playing_complete <playing_id>
    GmClearPlayingComplete(game_actor, command_args);
  } else if ("add_guild_light_attr" == command_args[0] ||
             "add_guild_dark_attr" == command_args[0] ||
             "add_guild_holy_attr" == command_args[0]) {
    // 转发 session_server
    session::protocol::MessageGmCommand session_message;
    session_message.__set_actor_id_(game_actor->GetActorID());
    session_message.__set_command_(message.command_);
    SessionChannel::SendMessage(session_message,
        session::protocol::GameMessageType::MESSAGE_GM_COMMAND);
  } else if("change_camp" == command_args[0] ||
            "cp" == command_args[0]) {
    // 改变阵营
    GmChangeCamp(game_actor, command_args);
  } else if ("add_buff" == command_args[0]) {
    // 增加BUFF
    // usage: add_buff <buff_id>
    GmAddBuff(game_actor, command_args);
  } else if ("add_hp" == command_args[0]) {
    // 增加HP
    // usage: add_hp <number>
    GmAddHp(game_actor, command_args);
  } else if ("add_mp" == command_args[0]) {
    // 增加MP
    // usage: add_mp <number>
    GmAddMp(game_actor, command_args);
  } else if("summon_npc" == command_args[0]) {
    // 召唤NPC
    // usage: summon_npc <id> <x> <y>
    GmSummonNpc(game_actor, command_args);
  } else if("clear_npc" == command_args[0]) {
    // 删除选中的NPC
    // usage: clear_npc
    GmClearNpc(game_actor, command_args);
  } else if("add" == command_args[0]) {
    // 添加
    GmAdd(game_actor, command_args);
  } else if("add_vip" == command_args[0]) {
    // 领取 VIP
    GmAddVip(game_actor, command_args);
  } else if ("set_fcm_type" == command_args[0]) {
    // 设置防沉迷认证类型
    // usage: set_fcm_type <fcm_type>
    GmSetFcmType(game_actor, command_args);
  } else if ("set_fcm_online_time" == command_args[0]) {
    // 设置防沉迷累计在线时间
    // usage: set_fcm_online_time <second>
    GmSetFcmOnlineTime(game_actor, command_args);
  } else if ("jump" == command_args[0]) {
    // 开地图
    // usage: jump <map_id> <x> <y>
    GmJump(game_actor, command_args);
  } else if ("transport" == command_args[0]) {
    // 护送
    // usage: transport <id>
    GmTransport(game_actor, command_args);
  } else if("add_resource" == command_args[0]) {
    // 添加资源
    GmAddResource(game_actor, command_args);
  } else {
    LOG_WARNING("Invalid command.");
  }
  
}

}  // namespace gm

}  // namespace server

}  // namespace game


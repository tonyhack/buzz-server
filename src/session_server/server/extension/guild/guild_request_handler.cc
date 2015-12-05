#include "session_server/server/extension/guild/guild_request_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "global/configure/configure.h"
#include "session_server/server/extension_manager.h"
#include "session_server/server/request/session_request_types.h"
#include "session_server/server/request/session_guild_request_types.h"
#include "session_server/server/extension/guild/guild.h"
#include "session_server/server/extension/guild/guild_manager.h"
#include "session_server/server/extension/guild/guild_actor.h"
#include "session_server/server/extension/guild/guild_actor_manager.h"

namespace session {

namespace server {

namespace guild {

GuildRequestHandler::GuildRequestHandler() {}
GuildRequestHandler::~GuildRequestHandler() {}

bool GuildRequestHandler::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_GUILD_GET_ID,
      boost::bind(&GuildRequestHandler::OnRequestGuildGetID, this, _1, _2));
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_GUILD_ADD_CONTRIBUTION,
      boost::bind(&GuildRequestHandler::OnRequestGuildAddContribution, this, _1, _2));
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_GUILD_ADD_ATTRIBUTE,
      boost::bind(&GuildRequestHandler::OnRequestGuildAddAttribute, this, _1, _2));
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_GUILD_GET_MEMBERS,
      boost::bind(&GuildRequestHandler::OnRequestGuildGetMembers, this, _1, _2));
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_GUILD_CHECK_AUTHORITY_CHAT,
      boost::bind(&GuildRequestHandler::OnRequestGuildCheckAuthorityChat, this, _1, _2));
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_GUILD_CHECK_CAN_CREATE_PLAYING,
      boost::bind(&GuildRequestHandler::OnRequestGuildCheckCanCreatePlaying, this, _1, _2));
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_GUILD_GET_CURRENT_PLAYING,
      boost::bind(&GuildRequestHandler::OnRequestGuildGetCurrentPlaying, this, _1, _2));
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_GUILD_SET_CURRENT_PLAYING,
      boost::bind(&GuildRequestHandler::OnRequestGuildSetCurrentPlaying, this, _1, _2));

  return true;
}

void GuildRequestHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_GUILD_GET_ID);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_GUILD_ADD_CONTRIBUTION);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_GUILD_ADD_ATTRIBUTE);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_GUILD_GET_MEMBERS);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_GUILD_CHECK_AUTHORITY_CHAT);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_GUILD_CHECK_CAN_CREATE_PLAYING);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_GUILD_GET_CURRENT_PLAYING);
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_GUILD_SET_CURRENT_PLAYING);
}

int GuildRequestHandler::OnRequestGuildGetID(void *message, size_t size) {
  if (NULL == message || size != sizeof(request::RequestGuildGetID)) {
    LOG_ERROR("Invalid params.");
    return -1;
  }

  request::RequestGuildGetID *request = (request::RequestGuildGetID *)message;

  GuildActor *guild_actor = GuildActorManager::GetInstance()->GetFromAll(request->actor_id_);
  if (NULL == guild_actor) {
    request->guild_id_ = 0;
  } else {
    request->guild_id_ = guild_actor->GetGuildID();
  }

  return 0;
}

int GuildRequestHandler::OnRequestGuildAddContribution(void *message, size_t size) {
  if (NULL == message || size != sizeof(request::RequestGuildAddContribution)) {
    LOG_ERROR("Invalid params.");
    return -1;
  }

  request::RequestGuildAddContribution *request =
    (request::RequestGuildAddContribution *)message;

  GuildActor *guild_actor = GuildActorManager::GetInstance()->GetFromAll(request->actor_id_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", request->actor_id_);
    return -1;
  }
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_ERROR("GuildActor(%lu) not join a guild.", request->actor_id_);
    return -1;
  }

  guild_actor->AddContribution(request->contribution_);

  return 0;
}

int GuildRequestHandler::OnRequestGuildCheckAuthorityChat(void *message, size_t size) {
  if (NULL == message || size != sizeof(request::RequestGuildCheckAuthorityChat)) {
    LOG_ERROR("Invalid params.");
    return -1;
  }

  request::RequestGuildCheckAuthorityChat *request =
    (request::RequestGuildCheckAuthorityChat *)message;

  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(request->actor_id_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", request->actor_id_);
    return -1;
  }
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    request->result_ = false;
    return 0;
  }

  const global::configure::GuildPositionCell *position_cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    return -1;
  }

  request->result_ = position_cell->authority_chat_;
  return 0;
}

int GuildRequestHandler::OnRequestGuildAddAttribute(void *message, size_t size) {
  if (NULL == message || size != sizeof(request::RequestGuildAddAttribute)) {
    LOG_ERROR("Invalid params.");
    return -1;
  }

  request::RequestGuildAddAttribute *request =
    (request::RequestGuildAddAttribute *)message;

  // 获取公会对象
  Guild *guild = NULL;

  if (request->__isset.guild_id_) {
    guild = GuildManager::GetInstance()->GetGuild(request->guild_id_);
    if (NULL == guild) {
      LOG_ERROR("Get Guild(%lu) from GuildManager failed.", request->guild_id_);
      return -1;
    }
  } else if (request->__isset.actor_id_) {
    GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(request->actor_id_);
    if (NULL == guild_actor) {
      LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", request->actor_id_);
      return -1;
    }
    guild = guild_actor->GetGuild();
    if (NULL == guild) {
      LOG_ERROR("GuildActor(%lu) not join a guild.", request->actor_id_);
      return -1;
    }
  } else {
    LOG_ERROR("actor_id and guild_id not set.");
    return -1;
  }

  if (request::GuildAttributeType::LIGHT == request->type_) {
    guild->SetLightAttribute(guild->GetLightAttribute() + request->value_);
  } else if (request::GuildAttributeType::DARK == request->type_) {
    guild->SetDarkAttribute(guild->GetDarkAttribute() + request->value_);
  } else if (request::GuildAttributeType::HOLY == request->type_) {
    guild->SetHolyAttribute(guild->GetHolyAttribute() + request->value_);
  } else {
    LOG_ERROR("Invalid type.");
    return -1;
  }

  return 0;
}

int GuildRequestHandler::OnRequestGuildGetMembers(void *message, size_t size) {
  if (NULL == message || size != sizeof(request::RequestGuildGetMembers)) {
    LOG_ERROR("Invalid params.");
    return -1;
  }

  request::RequestGuildGetMembers *request =
    (request::RequestGuildGetMembers *)message;

  GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(request->actor_id_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", request->actor_id_);
    return -1;
  }
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("GuildActor(%lu) Not join a guild.", request->actor_id_);
    return -1;
  }

  const Guild::GuildActorSet &members = guild->GetMembers();
  for (Guild::GuildActorSet::const_iterator iter = members.begin();
       iter != members.end(); ++iter) {
    const GuildActor *guild_actor = *iter;
    if (guild_actor != NULL) {
      request->members_.push_back(guild_actor->GetActorID());
    }
  }

  return 0;
}

int GuildRequestHandler::OnRequestGuildCheckCanCreatePlaying(void *message, size_t size) {
  if (NULL == message || size != sizeof(request::RequestGuildCheckCanCreatePlaying)) {
    LOG_ERROR("Invalid params.");
    return -1;
  }

  request::RequestGuildCheckCanCreatePlaying *request =
    (request::RequestGuildCheckCanCreatePlaying *)message;

  GuildActor *guild_actor = GuildActorManager::GetInstance()->GetFromAll(request->actor_id_);
  if (NULL == guild_actor) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", request->actor_id_);
    return -1;
  }

  // 检查是否有公会
  Guild *guild = guild_actor->GetGuild();
  if (NULL == guild) {
    LOG_WARNING("GuildActor(%lu) not join a guild.", request->actor_id_);
    request->result_ = false;
    return 0;
  }

  // 检查是否有创建公会副本的权限
  const global::configure::GuildPositionCell *position_cell =
    GUILD_CONF()->GetGuildPosition(guild_actor->GetGuildPosition());
  if (NULL == position_cell) {
    LOG_ERROR("GuildPositionCell(%d) not found in config.", guild_actor->GetGuildPosition());
    return -1;
  }
  if (!position_cell->authority_create_playing_) {
    LOG_WARNING("GuildActor(%lu) has no authority_create_playing.", request->actor_id_);
    request->result_ = false;
    return 0;
  }

  // 检查是否能够创建公会副本
  request->result_ = guild->CheckCanCreatePlaying(request->playing_template_id_);

  return 0;
}
int GuildRequestHandler::OnRequestGuildGetCurrentPlaying(void *message, size_t size) {
  if (NULL == message || size != sizeof(request::RequestGuildGetCurrentPlaying)) {
    LOG_ERROR("Invalid params.");
    return -1;
  }

  request::RequestGuildGetCurrentPlaying *request =
    (request::RequestGuildGetCurrentPlaying *)message;

  // 获取公会对象
  Guild *guild = NULL;

  if (request->__isset.guild_id_) {
    guild = GuildManager::GetInstance()->GetGuild(request->guild_id_);
    if (NULL == guild) {
      LOG_ERROR("Get Guild(%lu) from GuildManager failed.", request->guild_id_);
      return -1;
    }
  } else if (request->__isset.actor_id_) {
    GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(request->actor_id_);
    if (NULL == guild_actor) {
      LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", request->actor_id_);
      return -1;
    }
    guild = guild_actor->GetGuild();
    if (NULL == guild) {
      request->playing_id_ = 0;
      request->playing_template_id_ = 0;
      return 0;
    }
  } else {
    LOG_ERROR("actor_id and guild_id not set.");
    return -1;
  }

  request->playing_id_ = guild->GetCurrentPlayingID();
  request->playing_template_id_ = guild->GetCurrentPlayingTemplateID();

  return 0;
}

int GuildRequestHandler::OnRequestGuildSetCurrentPlaying(void *message, size_t size) {
  if (NULL == message || size != sizeof(request::RequestGuildSetCurrentPlaying)) {
    LOG_ERROR("Invalid params.");
    return -1;
  }

  request::RequestGuildSetCurrentPlaying *request =
    (request::RequestGuildSetCurrentPlaying *)message;

  // 获取公会对象
  Guild *guild = NULL;

  if (request->__isset.guild_id_) {
    guild = GuildManager::GetInstance()->GetGuild(request->guild_id_);
    if (NULL == guild) {
      LOG_ERROR("Get Guild(%lu) from GuildManager failed.", request->guild_id_);
      return -1;
    }
  } else if (request->__isset.actor_id_) {
    GuildActor *guild_actor = GuildActorManager::GetInstance()->Get(request->actor_id_);
    if (NULL == guild_actor) {
      LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", request->actor_id_);
      return -1;
    }
    guild = guild_actor->GetGuild();
    if (NULL == guild) {
      LOG_ERROR("GuildActor(%lu) not join a guild.", request->actor_id_);
      return -1;
    }
  } else {
    LOG_ERROR("actor_id and guild_id not set.");
    return -1;
  }

  guild->SetCurrentPlaying(request->playing_id_, request->playing_template_id_);

  return 0;
}

}  // namespace guild

}  // namespace server

}  // namespace session


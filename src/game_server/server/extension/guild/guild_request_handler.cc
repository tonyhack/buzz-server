//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-10-23 11:56:06.
// File name: guild_request_handler.cc
//
// Description:
// Define class GuildRequestHandler.
//

#include "game_server/server/extension/guild/guild_request_handler.h"

#include <boost/bind.hpp>

#include "game_server/server/extension_manager.h"
#include "game_server/server/request/game_guild_request_types.h"
#include "game_server/server/request/game_request_types.h"
#include "game_server/server/extension/guild/guild.h"
#include "game_server/server/extension/guild/guild_actor.h"
#include "game_server/server/extension/guild/guild_actor_manager.h"
#include "game_server/server/extension/guild/guild_manager.h"
#include "global/logging.h"

namespace game {

namespace server {

namespace guild {

GuildRequestHandler::GuildRequestHandler() {}
GuildRequestHandler::~GuildRequestHandler() {}

bool GuildRequestHandler::Initialize() {
  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_GUILD_GET_ACTOR_GUILD,
      boost::bind(&GuildRequestHandler::OnRequestGuildGetActorGuild, this, _1, _2));

  ExtensionManager::GetInstance()->SetRequestHandler(
      request::RequestType::REQUEST_GUILD_GET_ACTOR_GUILD_NAME,
      boost::bind(&GuildRequestHandler::OnRequestGuildGetActorGuildName, this, _1, _2));

  return true;
}

void GuildRequestHandler::Finalize() {
  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_GUILD_GET_ACTOR_GUILD);

  ExtensionManager::GetInstance()->ResetRequestHandler(
      request::RequestType::REQUEST_GUILD_GET_ACTOR_GUILD_NAME);
}

int GuildRequestHandler::OnRequestGuildGetActorGuild(void *message, size_t size) {
  if (NULL == message || size != sizeof(request::RequestGuildGetActorGuild)) {
    LOG_ERROR("Invalid params.");
    return -1;
  }

  request::RequestGuildGetActorGuild *request =
    (request::RequestGuildGetActorGuild *)message;

  GuildActor *actor = GuildActorManager::GetInstance()->Get(request->actor_id_);
  if (actor == NULL) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", request->actor_id_);
    return -1;
  }

  request->__set_guild_id_(actor->GetGuildID());

  return 0;
}

int GuildRequestHandler::OnRequestGuildGetActorGuildName(void *message, size_t size) {
  if (NULL == message || size != sizeof(request::RequestGuildGetActorGuildName)) {
    LOG_ERROR("Invalid params.");
    return -1;
  }

  request::RequestGuildGetActorGuildName *request =
    (request::RequestGuildGetActorGuildName *)message;

  GuildActor *actor = GuildActorManager::GetInstance()->Get(request->actor_id_);
  if (actor == NULL) {
    LOG_ERROR("Get GuildActor(%lu) from GuildActorManager failed.", request->actor_id_);
    return -1;
  }

  Guild *guild = GuildManager::GetInstance()->GetGuild(actor->GetGuildID());
  if(guild) {
    request->__set_guild_name_(guild->GetName());
  }

  return 0;
}

}  // namespace guild

}  // namespace server

}  // namespace game


#include "session_server/server/extension/guild/guild_daily_clean_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_guild_event_types.h"
#include "session_server/server/extension/guild/guild_manager.h"

namespace session {

namespace server {

namespace guild {

GuildDailyCleanEventHandler::GuildDailyCleanEventHandler() {}
GuildDailyCleanEventHandler::~GuildDailyCleanEventHandler() {}

bool GuildDailyCleanEventHandler::Initialize() {
  // Follow 公会每日清理事件
  this->conn_= coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_GUILD_DAILY_CLEAN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_SYSTEM,
      boost::bind(&GuildDailyCleanEventHandler::OnEvent, this, _1, _2, _3, _4));
  return true;
}

void GuildDailyCleanEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void GuildDailyCleanEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventGuildDailyClean) ||
      channel_type != event::ChannelType::CHANNEL_SYSTEM) {
    LOG_ERROR("Invalid params.");
    return;
  }

  GuildManager::GetInstance()->DailyClean();
}

}  // namespace guild

}  // namespace server

}  // namespace session


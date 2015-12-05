#include "session_server/server/extension/guild/guild_weekly_clean_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_guild_event_types.h"
#include "session_server/server/extension/guild/guild_manager.h"

namespace session {

namespace server {

namespace guild {

GuildWeeklyCleanEventHandler::GuildWeeklyCleanEventHandler() {}
GuildWeeklyCleanEventHandler::~GuildWeeklyCleanEventHandler() {}

bool GuildWeeklyCleanEventHandler::Initialize() {
  // Follow 公会每周清理事件
  this->conn_= coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_GUILD_WEEKLY_CLEAN, coresh::Communicator::GetChannelAll(),
      event::ChannelType::CHANNEL_SYSTEM,
      boost::bind(&GuildWeeklyCleanEventHandler::OnEvent, this, _1, _2, _3, _4));
  return true;
}

void GuildWeeklyCleanEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void GuildWeeklyCleanEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventGuildWeeklyClean) ||
      channel_type != event::ChannelType::CHANNEL_SYSTEM) {
    LOG_ERROR("Invalid params.");
    return;
  }

  GuildManager::GetInstance()->WeeklyClean();
}

}  // namespace guild

}  // namespace server

}  // namespace session


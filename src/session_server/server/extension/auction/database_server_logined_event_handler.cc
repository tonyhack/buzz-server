#include "session_server/server/extension/auction/database_server_logined_event_handler.h"

#include <boost/bind.hpp>

#include "global/logging.h"
#include "session_server/server/event/session_event_types.h"
#include "session_server/server/event/session_system_event_types.h"
#include "session_server/server/extension/auction/auction_transaction_manager.h"

namespace session {

namespace server {

namespace auction {

DatabaseServerLoginedEventHandler::DatabaseServerLoginedEventHandler() {}
DatabaseServerLoginedEventHandler::~DatabaseServerLoginedEventHandler() {}

bool DatabaseServerLoginedEventHandler::Initialize() {
  this->conn_ = coresh::CommunicatorSingleton::GetInstance().Follow(
      event::EventType::EVENT_SYSTEM_DATABASE_SERVER_LOGINED, 0,
      event::ChannelType::CHANNEL_SYSTEM,
      boost::bind(&DatabaseServerLoginedEventHandler::OnEvent, this, _1, _2, _3, _4));

  return true;
}

void DatabaseServerLoginedEventHandler::Finalize() {
  coresh::CommunicatorSingleton::GetInstance().Unfollow(this->conn_);
}

void DatabaseServerLoginedEventHandler::OnEvent(core::uint64 channel, core::uint8 channel_type,
    const void *message, size_t size) {
  if (NULL == message || size != sizeof(event::EventSystemDatabaseServerLogined) ||
      channel_type != event::ChannelType::CHANNEL_SYSTEM) {
    LOG_ERROR("Invalid params.");
    return;
  }

  // 建立交易索引, 处理到期的交易
  AuctionTransactionManager::GetInstance()->BuildIndex();
}

}  // namespace auction

}  // namespace server

}  // namespace session


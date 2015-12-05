//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-16 15:36:00.
// File name: async_request_loop.h
//
// Description:
// Define class AsyncRequestLoop.
//

#ifndef __DATABASE__SERVER__ASYNC__REQUEST__LOOP__H
#define __DATABASE__SERVER__ASYNC__REQUEST__LOOP__H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "core/async_logging_singleton.h"
#include "core/base_loop.h"
#include "core/base/byte_buffer_static.h"
#include "coresh/compressor.h"
#include "coresh/encoder.h"
#include "coresh/packetizer.h"
#include "database_server/protocol/async_request_types.h"
#include "database_server/protocol/database_protocol_types.h"
#include "database_server/protocol/database_storage_types.h"
#include "database_server/server/database_server.h"
#include "database_server/server/database_terminal.h"
#include "database_server/server/mysql_client.h"
#include "global/thrift_packet.h"


// 按照database_storage.thrift和server_sample.xml定义的存储过程顺序排序
#include "database_server/server/storage_check_session.h"
#include "database_server/server/storage_test_echo.h"
#include "database_server/server/storage_actor_online.h"
#include "database_server/server/storage_actor_logout_center.h"
#include "database_server/server/storage_actor_login.h"
#include "database_server/server/storage_actor_save.h"
#include "database_server/server/storage_cooling_login.h"
#include "database_server/server/storage_cooling_save.h"
#include "database_server/server/storage_skill_login.h"
#include "database_server/server/storage_skill_save.h"
#include "database_server/server/storage_buff_login.h"
#include "database_server/server/storage_buff_save.h"
#include "database_server/server/storage_item_actor_login.h"
#include "database_server/server/storage_item_actor_save.h"
#include "database_server/server/storage_item_load.h"
#include "database_server/server/storage_item_save.h"
#include "database_server/server/storage_gem_login.h"
#include "database_server/server/storage_gem_save.h"
#include "database_server/server/storage_soul_load.h"
#include "database_server/server/storage_soul_save.h"
#include "database_server/server/storage_soul_actor_load.h"
#include "database_server/server/storage_soul_actor_save.h"
#include "database_server/server/storage_task_load.h"
#include "database_server/server/storage_task_save.h"
#include "database_server/server/storage_task_actor_load.h"
#include "database_server/server/storage_task_actor_save.h"
#include "database_server/server/storage_attr_actor_login.h"
#include "database_server/server/storage_attr_actor_save.h"
#include "database_server/server/storage_spirit_facade_login.h"
#include "database_server/server/storage_spirit_facade_save.h"
#include "database_server/server/storage_mail_add.h"
#include "database_server/server/storage_mail_login.h"
#include "database_server/server/storage_mail_del.h"
#include "database_server/server/storage_mail_pick_affix.h"
#include "database_server/server/storage_mail_read.h"
#include "database_server/server/storage_mail_affix_state.h"
#include "database_server/server/storage_social_actor_login.h"
#include "database_server/server/storage_social_actor_save.h"
#include "database_server/server/storage_guild_del.h"
#include "database_server/server/storage_guild_save.h"
#include "database_server/server/storage_guild_actor_login.h"
#include "database_server/server/storage_guild_actor_save.h"
#include "database_server/server/storage_playing_actor_login.h"
#include "database_server/server/storage_playing_actor_save.h"
#include "database_server/server/storage_auction_actor_login.h"
#include "database_server/server/storage_auction_actor_save.h"
#include "database_server/server/storage_auction_transaction_del.h"
#include "database_server/server/storage_auction_transaction_save.h"
#include "database_server/server/storage_achieve_save.h"
#include "database_server/server/storage_achieve_login.h"
#include "database_server/server/storage_welfare_load.h"
#include "database_server/server/storage_welfare_save.h"
#include "database_server/server/storage_world_data_save.h"

namespace core {

class BoundlessQueue;

}  // namespace core

namespace database {

namespace server {

class AsyncRequestLoop : public core::BaseLoop {
  typedef boost::shared_ptr<core::BoundlessQueue> BoundlessQueuePtr;
  typedef boost::function<void (const protocol::AsyncRequestMessage &)> MessageHandler;

 public:
  AsyncRequestLoop();
  virtual ~AsyncRequestLoop();

  bool Initialize(size_t queue_size, int queue_reduce_waterline,
      coresh::CompressType::Type compress, coresh::EncodeType::Type encode);

  bool Start();
  void Stop();

  // Message handler.
  inline void SetMessageHandler(protocol::StorageType::type type,
      const MessageHandler &handler) {
    this->message_handler_[type] = handler;
  }

  // Get mysql session client.
  inline MysqlSessionPtr GetMysqlSession(core::uint64 channel) {
    return this->mysqls_.GetSession(channel);
  }
  inline MysqlSessionPtr GetCenterMysqlSession() {
    return this->mysqls_.GetCenterSession();
  }

  // Get thrift packet.
  inline global::ThriftPacket *GetPacket() {
    return &this->packet_;
  }

  // Do request.
  bool Request(const char *message, size_t size);

  // Send response.
  template <typename T>
  void SendResponse(const T &response, protocol::ResponseType::type result,
      const protocol::AsyncRequestMessage &request);

  virtual void Loop();
  virtual void Wakeup();

 private:
  void HandleRequest(const char *message, size_t size);

  BoundlessQueuePtr queue_;

  global::ThriftPacket packet_;
  coresh::Packetizer packetizer_;
  core::ByteBufferStaticMalloc buffer_;

  MysqlClient mysqls_;

  MessageHandler message_handler_[protocol::StorageType::STORAGE_MAX];

  // Storage:
  // 按照database_storage.thrift和server_sample.xml定义的存储过程顺序排序
  StorageTestEcho test_echo_;
  StorageCheckSession check_session_;
  StorageActorOnline actor_online_;
  StorageActorLogoutCenter actor_logout_center_;

  StorageActorLogin actor_login_;
  StorageActorSave actor_save_;

  StorageCoolingLogin cooling_login_;
  StorageCoolingSave cooling_save_;

  StorageSkillLogin skill_login_;
  StorageSkillSave skill_save_;

  StorageBuffLogin buff_login_;
  StorageBuffSave buff_save_;

  StorageItemActorLogin item_actor_login_;
  StorageItemActorSave item_actor_save_;
  StorageItemLoad item_load_;
  StorageItemSave item_save_;
  StorageGemLogin gem_login_;
  StorageGemSave gem_save_;

  StorageSoulLoad soul_load_;
  StorageSoulSave soul_save_;
  StorageSoulActorLoad soul_actor_load_;
  StorageSoulActorSave soul_actor_save_;

  StorageTaskLoad task_load_;
  StorageTaskSave task_save_;
  StorageTaskActorLoad task_actor_load_;
  StorageTaskActorSave task_actor_save_;

  StorageAttrActorLogin attr_actor_login_;
  StorageAttrActorSave attr_actor_save_;
  StorageSpiritFacadeLogin spirit_facade_login_;
  StorageSpiritFacadeSave spirit_facade_save_;

  StorageMailAdd mail_add_;
  StorageMailLogin mail_login_;
  StorageMailDel mail_del_;
  StorageMailAffix mail_affix_;
  StorageMailRead mail_read_;
  StorageMailAffixState mail_affix_state_;

  StorageSocialActorLogin social_actor_login_;
  StorageSocialActorSave social_actor_save_;

  StorageGuildDel guild_del_;
  StorageGuildSave guild_save_;
  StorageGuildActorLogin guild_actor_login_;
  StorageGuildActorSave guild_actor_save_;

  StoragePlayingActorLogin playing_actor_login_;
  StoragePlayingActorSave playing_actor_save_;

  StorageAuctionActorLogin auction_actor_login_;
  StorageAuctionActorSave auction_actor_save_;
  StorageAuctionTransactionDel auction_transaction_del_;
  StorageAuctionTransactionSave auction_transaction_save_;

  StorageAchieveLogin achieve_load_;
  StorageAchieveSave achieve_save_;
  StorageWelfareLoad welfare_load_;
  StorageWelfareSave welfare_save_;

  StorageWorldDataSave world_data_;
};

template <typename T>
void AsyncRequestLoop::SendResponse(const T &response, protocol::ResponseType::type result,
    const protocol::AsyncRequestMessage &request) {
  size_t size = 0;
  const char *message = this->packet_.Serialize(response, size);
  if(message == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Serialize message failed.", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::MessageStorageResponse message2;
  message2.request_.__set_type_(request.type_);
  message2.request_.__set_message_(request.message_);
  message2.request_.__set_channel_(request.channel_);
  message2.__set_result_(result);
  message2.response_.assign(message, size);
  message = this->packet_.Serialize(message2, size);
  if(message == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Serialize message failed.", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  protocol::Message message3;
  message3.__set_type_(protocol::MessageType::MESSAGE_STORAGE_RESPONSE);
  message3.message_.assign(message, size);
  message = this->packet_.Serialize(message3, size);
  if(message == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Serialize message failed.", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  DatabaseTerminalPtr terminal =
    DatabaseServerSingleton::GetInstance().GetTerminals().GetTerminal(
        request.terminal_key_.type_, request.terminal_key_.id_);
  if(terminal.get() == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Get database terminal failed.", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  terminal->SendMessage(message, size, &this->packetizer_);
}

}  // namespace server

}  // namespace database

#endif  // __DATABASE__SERVER__ASYNC__REQUEST__LOOP__H


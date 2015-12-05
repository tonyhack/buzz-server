//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-01-16 16:44:48.
// File name: async_request_loop.cc
//
// Description:
// Define class AsyncRequestLoop.
//

#include "database_server/server/async_request_loop.h"

#include "core/base/boundless_queue.h"
#include "coresh/variable_table.h"
#include "global/logging.h"

namespace database {

namespace server {

AsyncRequestLoop::AsyncRequestLoop()
  : buffer_(coresh::VariableTableSingleton::GetInstance().GetMaxMessageSize()) {}

AsyncRequestLoop::~AsyncRequestLoop() {}

bool AsyncRequestLoop::Initialize(size_t queue_size, int queue_reduce_waterline,
    coresh::CompressType::Type compress, coresh::EncodeType::Type encode) {
  this->packetizer_.SetCompressor(compress);
  this->packetizer_.SetEncoder(encode);

  // Allocate boundles queue.
  this->queue_.reset(new (std::nothrow) core::BoundlessQueue(queue_size,
        core::kBlockingAllPattern, queue_reduce_waterline));
  if(this->queue_.get() == NULL) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Allocate boundless queue failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  // Initialize and start mysqls.
  if(this->mysqls_.Initialize() == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize mysqls failed.", __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->test_echo_.Initialize(this) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize storage test echo failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->check_session_.Initialize(this) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Initialize storage check session failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->actor_login_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage actor login failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->actor_logout_center_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage actor logout center failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->actor_online_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage actor online failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->actor_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage actor save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->cooling_login_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage cooling login failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->cooling_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage cooling save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->skill_login_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage skill login failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->skill_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage skill save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->buff_login_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage cooling login failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->buff_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage cooling save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->item_actor_login_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage item_actor login failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->item_actor_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage item_actor save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->item_load_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage item load failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->item_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage item save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->gem_login_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage gem login failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->gem_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage gem save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->soul_load_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage soul load failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->soul_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage soul save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->soul_actor_load_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage soul actor load failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->soul_actor_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage soul actor save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->task_load_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage task load failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->task_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage task save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  if(this->task_actor_load_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage task_actor load failed.",
        __FILE__, __LINE__, __FUNCTION__);
  }
  if(this->task_actor_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage task_actor save failed.",
    __FILE__, __LINE__, __FUNCTION__);
  }
  if(this->attr_actor_login_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage attr_actor login failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->attr_actor_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage attr_actor save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->spirit_facade_login_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage spirit_facade login failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->spirit_facade_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage spirit_facade save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->mail_add_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage mail_add save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->mail_login_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage mail_login  failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
  
  if(this->mail_del_.Initialize(this) == false) {
     global::LogError("%s:%d (%s) Initialize storage mail_del  failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->mail_affix_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage mail_affix  failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->mail_read_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage mail_read  failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->mail_affix_state_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage mail_affix_state  failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->social_actor_login_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage social_actor login failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->social_actor_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage social_actor save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->guild_del_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage guild del failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->guild_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage guild save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->guild_actor_login_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage guild_actor login failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->guild_actor_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage guild_actor save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->playing_actor_login_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage playing_actor login failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->playing_actor_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage playing_actor save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->auction_actor_login_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage auction_actor login failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->auction_actor_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage auction_actor save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->auction_transaction_del_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage auction_transaction del failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->auction_transaction_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage auction_transaction save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->achieve_load_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage achieve load failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->achieve_save_.Initialize(this) == false) {
    global::LogError("%s:%d (%s) Initialize storage achieve save failed.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }

  if(this->welfare_load_.Initialize(this) == false) {
    LOG_ERROR("Initialize storage welfare load failed.");
    return false;
  }

  if(this->welfare_save_.Initialize(this) == false) {
    LOG_ERROR("Initialize storage welfare save failed.");
    return false;
  }

  if(this->world_data_.Initialize(this) == false) {
    LOG_ERROR("Initialize storage world data save failed.");
    return false;
  }

  return true;
}

bool AsyncRequestLoop::Start() {
  return this->mysqls_.Start();
}

void AsyncRequestLoop::Stop() {
  return this->mysqls_.Stop();
}

void AsyncRequestLoop::Loop() {
  this->looping_ = true;
  int ret = 0;

  while(this->quit_ == false) {
    ret = this->queue_->Remove((char *)this->buffer_.BufferPointer(),
        coresh::VariableTableSingleton::GetInstance().GetMaxMessageSize());
    if(ret >= 0) {
      this->HandleRequest((const char *)this->buffer_.BufferPointer(), ret);
    } else if(ret < 0 && this->quit_ == false) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Remove from queue failed[%d]",
          __FILE__, __LINE__, __FUNCTION__, ret);
      break;
    }
  }

  this->looping_ = false;
}

void AsyncRequestLoop::Wakeup() {
  this->queue_->Terminate();
}

bool AsyncRequestLoop::Request(const char *message, size_t size) {
  if(this->looping_ == true) {
    int ret = 0;
    ret = this->queue_->Add(message, size);
    if(ret != (int)size) {
      AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
          "%s:%d (%s) Add bound queue failed [%d].",
          __FILE__, __LINE__, __FUNCTION__, ret);
      return false;
    }
    return true;
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Add bound queue failed, looping end.",
        __FILE__, __LINE__, __FUNCTION__);
    return false;
  }
}

void AsyncRequestLoop::HandleRequest(const char *message, size_t size) {
  protocol::AsyncRequestMessage message2;
  if(this->packet_.Deserialize(message2, message, size) == false) {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Deserialize message failed", __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  if(message2.type_ < protocol::StorageType::STORAGE_MAX &&
      this->message_handler_[message2.type_]) {
    this->message_handler_[message2.type_](message2);
  } else {
    AsyncLoggingSingleton::GetAsyncLogging()->Logging(core::LOGGING_LEVEL_ERROR,
        "%s:%d (%s) Handle request[%d] failed.",
        __FILE__, __LINE__, __FUNCTION__, message2.type_);
  }
}

}  // namespace server

}  // namespace database


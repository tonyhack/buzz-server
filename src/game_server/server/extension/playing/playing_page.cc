//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-25 14:14:25.
// File name: playing_page.h
//
// Description:
// Define class PlayingPage.
//

#include "game_server/server/extension/playing/playing_page.h"

#include "coresh/communicator.h"
#include "game_server/server/session_channel.h"
#include "game_server/server/event/game_event_types.h"
#include "game_server/server/event/game_playing_event_types.h"
#include "gateway_server/protocol/gateway_playing_protocol_types.h"
#include "global/logging.h"
#include "session_server/protocol/game_playing_protocol_types.h"
#include "session_server/protocol/session_game_protocol_types.h"

namespace game {

namespace server {

namespace playing {

PlayingPage::PlayingPage() {}

PlayingPage::~PlayingPage() {}

bool PlayingPage::Initialize(PlayingActor *actor, core::int32 template_id) {
  if (actor == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  this->SetTemplate(template_id);
  this->actor_ = actor;
  this->result_.__set_awarded_(true);
  this->record_.__set_first_pass_awarded_(false);

  return true;
}

void PlayingPage::Finalize() {
  this->record_.values_.clear();
  this->result_.values_.clear();
  this->SetTemplate(0);
  this->actor_ = NULL;
}

bool PlayingPage::Save(std::string &record, std::string &result) {
  char buffer[128] = "";
  size_t count = 0;
  int ret = 0;

  // record
  count += snprintf(buffer + count, sizeof(buffer) - count , "%d+%d+%d+%d+%d+",
      this->GetTemplate(), this->record_.period_count_,
      this->record_.spend_secs_,
      this->record_.first_pass_awarded_ == true ? 1 : 0,
      (int)this->record_.values_.size());
  for (size_t i = 0; i < this->record_.values_.size(); ++i) {
    ret = snprintf(buffer + count, sizeof(buffer) - count, "%d+",
        this->record_.values_[i]);
    if (ret < 0) {
      LOG_ERROR("玩家(%lu) 保存PlayingPage(%d) 出错",
          this->actor_->GetID(), this->GetTemplate());
      return false;
    }
    count += ret;
  }
  record.assign(buffer, count);

  count = 0;
  // result
  count += snprintf(buffer + count, sizeof(buffer) - count , "%d+%d+%d+%d+%d+",
      this->GetTemplate(), this->result_.awarded_ == true ? 1 : 0,
      this->result_.award_count_, this->result_.paid_award_count_,
      (int)this->result_.values_.size());
  for (size_t i = 0; i < this->result_.values_.size(); ++i) {
    ret = snprintf(buffer + count, sizeof(buffer) - count, "%d+",
        this->result_.values_[i]);
    if (ret < 0) {
      LOG_ERROR("玩家(%lu) 保存PlayingPage(%d) 出错",
          this->actor_->GetID(), this->GetTemplate());
      return false;
    }
    count += ret;
  }
  result.assign(buffer, count);

  return true;
}

void PlayingPage::SetAwarded() {
  this->result_.__set_awarded_(true);
}

void PlayingPage::SetFirstPassAwarded() {
  this->record_.__set_first_pass_awarded_(true);
}

void PlayingPage::Synchronize() const {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return;
  }

  // 同步玩家
  gateway::protocol::MessagePlayingRecordSynchronize message;
  message.__set_playing_template_(this->GetTemplate());
  message.__set_record_(this->GetRecord());
  message.__set_result_(this->GetResult());
  this->actor_->SendMessage(message,
      gateway::protocol::MessageType::MESSAGE_PLAYING_RECORD_SYNCHRONIZE);

  // 同步会话
  session::protocol::PlayingPageField page;
  page.__set_template_(this->GetTemplate());
  page.__set_record_(this->GetRecord());
  page.__set_result_(this->GetResult());
  session::protocol::MessagePlayingActorPageSynchronize synchronize;
  synchronize.__set_actor_(this->actor_->GetID());
  synchronize.pages_.push_back(page);
  SessionChannel::SendMessage(synchronize,
      session::protocol::GameMessageType::MESSAGE_PLAYING_ACTOR_PAGE_SYNCHRONIZE);
}

bool PlayingPage::FinishAutoPage(core::int32 finish_count) {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return false;
  }

  event::EventPlayingRequestAutoFinish event;
  event.__set_actor_(this->actor_->GetID());
  event.__set_template_(this->GetTemplate());
  event.__set_record_(this->GetRecord());
  event.__set_finish_count_(finish_count);
  coresh::CommunicatorSingleton::GetInstance().Broadcast(
      event::EventType::EVENT_PLAYING_REQUEST_AUTO_FINISH, this->actor_->GetID(),
      event::ChannelType::CHANNEL_ACTOR, &event, sizeof(event));

  return true;
}

}  // namespace playing

}  // namespace server

}  // namespace game


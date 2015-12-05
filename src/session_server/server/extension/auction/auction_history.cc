#include "session_server/server/extension/auction/auction_history.h"

#include <cstdio>
#include <cstring>

#include "entity/auction_types.h"
#include "global/logging.h"
#include "global/common_functions.h"
#include "session_server/server/global_time_tick.h"
#include "session_server/server/brief_actor.h"
#include "session_server/server/session_actor_manager.h"

namespace session {

namespace server {

namespace auction {

AuctionHistory::AuctionHistory() : type_(0) {
}

AuctionHistory::AuctionHistory(core::int32 type) : type_(type) {
  this->timestamp_ =
    global::ToString(GlobalTimeTick::GetInstance()->TimeTick().GetRealSec());
}

AuctionHistory::~AuctionHistory() {
}

bool AuctionHistory::Initialize(const std::string &data) {
  std::vector<std::string> split_result;
  global::StringSplit(data.c_str(), ":", split_result);

  if (split_result.size() < 2) {
    LOG_ERROR("Event data is invalid.");
    return false;
  }

  this->type_ = atoi(split_result[0].c_str());
  this->timestamp_ = split_result[1].c_str();

  for (size_t i = 2; i < split_result.size(); ++i) {
    this->PushHistoryVar(split_result[i]);
  }

  return true;
}

void AuctionHistory::PushHistoryVar(const std::string &event_var) {
  this->history_vars_.push_back(event_var);
}

void AuctionHistory::PushHistoryVar(core::uint64 event_var) {
  this->history_vars_.push_back(global::ToString(event_var));
}

void AuctionHistory::PushHistoryVar(core::uint32 event_var) {
  this->history_vars_.push_back(global::ToString(event_var));
}

void AuctionHistory::PushHistoryVar(core::int32 event_var) {
  this->history_vars_.push_back(global::ToString(event_var));
}

void AuctionHistory::TranslateHistoryVar(std::vector<std::string> *result) const {
  // 0 no translate
  // 1 actor_id -> actor_name
  static const char *translate_table[] = {
    "1000",                  // BID_ITEM_SUCCESS
    "1000",                  // BUYOUT_ITEM_SUCCESS
    "000000",                // SELL_ITEM_BID_SUCCESS
    "000000",                // SELL_ITEM_BUYOUT_SUCCESS
    "1000",                  // BID_ITEM
    "1000",                  // OUTBIDDED
  };

  for (size_t i = 0; i < this->history_vars_.size(); ++i) {
    if ((size_t)this->type_ < sizeof(translate_table) / sizeof(char *) &&
        i < strlen(translate_table[this->type_])) {
      // 转换为玩家名
      if (translate_table[this->type_][i] == '1') {
        BriefActor *brief_actor =
          SessionActorManager::GetInstance()->GetBriefActor(atol(this->history_vars_[i].c_str()));
        if (NULL == brief_actor) {
          LOG_ERROR("Get BriefActor(%s) from SessionActorManager failed.",
                    this->history_vars_[i].c_str());
          return;
        }
        result->push_back(brief_actor->GetName());
      } else {
        result->push_back(this->history_vars_[i]);
      }
    } else {
      LOG_ERROR("Invalid type[%d] or index[%zd].", this->type_, i);
      return;
    }
  }
}

}  // namespace auction

}  // namespace server

}  // namespace session


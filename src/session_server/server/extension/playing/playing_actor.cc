//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-16 09:28:09.
// File name: playing_actor.cc
//
// Description:
// Define class PlayingActor.
//

#include "session_server/server/extension/playing/playing_actor.h"

#include "global/logging.h"
#include "global/configure/configure.h"
#include "global/configure/playing_configure.h"
#include "session_server/server/extension/playing/playing_page.h"
#include "session_server/server/extension/playing/playing_page_pool.h"

namespace session {

namespace server {

namespace playing {

PlayingActor::PlayingActor() {}
PlayingActor::~PlayingActor() {}

bool PlayingActor::Initialize(SessionActor *actor) {
  if(actor == NULL) {
    LOG_ERROR("参数为 NULL");
    return false;
  }

  if (this->playing_team_actor_.Initialize(this) == false) {
    LOG_ERROR("初始化 PlayingTeamActor 失败");
    return false;
  }

  this->actor_ = actor;
  this->playing_ = 0;

  return true;
}

void PlayingActor::Finalize() {
  PageMap::iterator iterator = this->pages_.begin();
  for(; iterator != this->pages_.end(); ++iterator) {
    if (iterator->second) {
      iterator->second->Finalize();
      PlayingPagePool::GetInstance()->Deallocate(iterator->second);
    }
  }
  this->pages_.clear();

  this->playing_team_actor_.Finalize();

  this->actor_ = NULL;
  this->playing_ = 0;
}

PlayingPage *PlayingActor::GetPage(core::int32 template_id) {
  PageMap::iterator iterator = this->pages_.find(template_id);
  if (iterator != this->pages_.end()) {
    return iterator->second;
  }
  return NULL;
}

const PlayingPage *PlayingActor::GetPage(core::int32 template_id) const {
  PageMap::const_iterator iterator = this->pages_.find(template_id);
  if (iterator != this->pages_.end()) {
    return iterator->second;
  }
  return NULL;
}

bool PlayingActor::AddPage(PlayingPage *page) {
  if (page == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  PageMap::const_iterator iterator = this->pages_.find(page->GetTemplate());
  if (iterator != this->pages_.end()) {
    return false;
  }

  this->pages_.insert(std::make_pair(page->GetTemplate(), page));

  return true;
}

PlayingPage *PlayingActor::RemovePage(core::int32 template_id) {
  PlayingPage *page = NULL;
  PageMap::iterator iterator = this->pages_.find(template_id);
  if (iterator != this->pages_.end()) {
    page = iterator->second;
    this->pages_.erase(iterator);
  }
  return page;
}

core::int32 PlayingActor::GetPlayingAvailableCount(core::int32 template_id) const {
  if (this->actor_ == NULL) {
    LOG_ERROR("actor_ 为空");
    return 0;
  }

  BriefActor *brief_actor = this->actor_->GetBriefActor();
  if (brief_actor == NULL) {
    LOG_ERROR("获取 BriefActor 失败");
    return 0;
  }

  const global::configure::PlayingCell *cell =
    global::configure::ConfigureSingleton::GetInstance().GetPlayingCell(template_id);
  if(cell == NULL) {
    LOG_ERROR("获取 PlayingCell(%d) 配置失败", template_id);
    return 0;
  }

  if(cell->depend_ != 0) {
    const PlayingPage *depend_page = this->GetPage(cell->depend_);
    if (depend_page == NULL || depend_page->GetRecord().spend_secs_ <= 0) {
      return 0;
    }
  }

  core::int32 vip_period_count = 0;
  const global::configure::VipCell *vip_cell =
    brief_actor->GetVipPrivilege(entity::VipPrivilegeType::PLAYING_PERIOD_COUNT, cell->id_);
  if (vip_cell != NULL) {
    vip_period_count = vip_cell->value2_;
  }

  const PlayingPage *page = this->GetPage(template_id);
  if (page == NULL) {
    return cell->period_count_ + vip_period_count;
  } else {
    const entity::PlayingRecordField &record = page->GetRecord();
    if (record.period_count_ >= cell->period_count_ + vip_period_count) {
      return 0;
    } else {
      return cell->period_count_ + vip_period_count;
    }
  }
}

}  // namespace playing

}  // namespace server

}  // namespace session


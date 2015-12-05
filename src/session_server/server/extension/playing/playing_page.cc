//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-26 18:12:38.
// File name: playing_page.cc
//
// Description:
// Define class PlayingPage.
//

#include "session_server/server/extension/playing/playing_page.h"

#include "global/logging.h"

namespace session {

namespace server {

namespace playing {

PlayingPage::PlayingPage() {}
PlayingPage::~PlayingPage() {}

bool PlayingPage::Initialize(PlayingActor *actor, core::int32 template_id) {
  if (actor == NULL) {
    LOG_ERROR("参数错误");
    return false;
  }

  this->actor_ = actor;
  this->SetTemplate(template_id);

  return true;
}

void PlayingPage::Finalize() {
  this->record_.values_.clear();
  this->result_.values_.clear();
  this->SetTemplate(0);
  this->actor_ = NULL;
}

}  // namespace playing

}  // namespace server

}  // namespace session


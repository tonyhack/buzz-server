//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-03-26 03:16:53.
// File name: playing_page.h
//
// Description:
// Define class PlayingPage.
//

#ifndef __SESSION__SERVER__PLAYING__PLAYING__PAGE__H
#define __SESSION__SERVER__PLAYING__PLAYING__PAGE__H

#include "entity/playing_types.h"
#include "session_server/server/extension/playing/playing_actor.h"

namespace session {

namespace server {

namespace playing {

class PlayingPage : public core::Noncopyable {
 public:
  PlayingPage();
  ~PlayingPage();

  bool Initialize(PlayingActor *actor, core::int32 template_id);
  void Finalize();

  inline core::int32 GetTemplate() const {
    return this->template_;
  }
  inline void SetTemplate(core::int32 template_id) {
    this->template_ = template_id;
  }

  inline const entity::PlayingResultField &GetResult() const {
    return this->result_;
  }
  inline void SetResult(const entity::PlayingResultField &result) {
    this->result_ = result;
  }

  inline const entity::PlayingRecordField &GetRecord() const {
    return this->record_;
  }
  inline void SetRecord(const entity::PlayingRecordField &record) {
    this->record_ = record;
  }

 private:
  PlayingActor *actor_;
  core::int32 template_;
  entity::PlayingResultField result_;
  entity::PlayingRecordField record_;
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__PLAYING__PAGE__H


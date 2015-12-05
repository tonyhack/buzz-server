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

#ifndef __GAME__SERVER__PLAYING__PLAYING__PAGE__H
#define __GAME__SERVER__PLAYING__PLAYING__PAGE__H

#include "database_server/protocol/storage_playing_actor_login_types.h"
#include "entity/playing_types.h"
#include "game_server/server/extension/playing/playing_actor.h"

namespace game {

namespace server {

namespace playing {

class PlayingPage {
 public:
  PlayingPage();
  ~PlayingPage();

  bool Initialize(PlayingActor *actor, core::int32 template_id);
  void Finalize();

  bool Save(std::string &record, std::string &result);

  inline core::int32 GetTemplate() const {
    return this->template_;
  }
  inline void SetTemplate(core::int32 template_id) {
    this->template_ = template_id;
  }

  inline const entity::PlayingResultField &GetResult() const {
    return this->result_;
  }
  inline entity::PlayingResultField &GetResult() {
    return this->result_;
  }
  inline void SetResult(const entity::PlayingResultField &result) {
    this->result_ = result;
  }

  inline const entity::PlayingRecordField &GetRecord() const {
    return this->record_;
  }
  inline entity::PlayingRecordField &GetRecord() {
    return this->record_;
  }
  inline void SetRecord(const entity::PlayingRecordField &record) {
    this->record_ = record;
  }

  // 设置领取过奖励
  void SetAwarded();
  void SetFirstPassAwarded();

  // 同步副本记录
  void Synchronize() const;

  // 完成自动进行
  bool FinishAutoPage(core::int32 finish_count);

 private:
  PlayingActor *actor_;

  // 模板ID
  core::int32 template_;
  // 上次结果
  entity::PlayingResultField result_;
  // 历史记录
  entity::PlayingRecordField record_;
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYING__PLAYING__PAGE__H


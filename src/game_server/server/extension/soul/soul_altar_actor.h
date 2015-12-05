#ifndef __GAME__SERVER__SOUL__SOUL__ALTAR__ACTOR__H
#define __GAME__SERVER__SOUL__SOUL__ALTAR__ACTOR__H

#include <string>
#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace database { namespace protocol {
class StorageSoulActorField;
}}

namespace game {

namespace server {

namespace soul {

class SoulActor;

class SoulAltarCard {
 public:
  SoulAltarCard() {}
  ~SoulAltarCard() {}

  bool Initialize(const std::string &data);
  int Serialize(char *buff, size_t size) const;

  core::uint32 card_id_;
  bool exist_;
};

class SoulAltarActor : public core::Noncopyable {
 public:
  static const core::int32 kMaxCardSlotNum = 3;
  typedef std::vector<SoulAltarCard> SoulAltarCardVector;

  SoulAltarActor();
  ~SoulAltarActor();

  bool Initialize(SoulActor *soul_actor,
      const database::protocol::StorageSoulActorField &db_field);
  void Finalize();
  void Save(database::protocol::StorageSoulActorField &db_field);

  // 开启功能
  bool EnableSoulAltarFunc(bool synchronize = true);

  bool CheckSummonPaid() const { return this->summon_paid_; }
  void SetSummonPaid(bool synchronize = true);
  bool CheckCardBind() const { return this->card_bind_; }
  void SetCardBind(bool card_bind) { this->card_bind_ = card_bind; }

  core::uint32 GetCardID(core::int32 pos) const;
  core::int32 GetLeftCardCount() const { return this->left_card_count_; }
  core::int32 GetFreeSummonTimes() const { return this->free_summon_times_; }
  void AddFreeSummonTimes(bool synchronize = true);

  void SummonCards(bool card_bind, core::int32 hint_pos, bool synchronize = true);
  void TakeCard(core::int32 pos, bool synchronize = true);
  void ResetCards(bool synchronize = true);

  // 每日清理
  void DailyClean(bool synchronize = true);

 private:
  void SendInitToClient();

  SoulActor *soul_actor_;

  bool summon_paid_;
  core::int32 free_summon_times_;
  SoulAltarCardVector cards_;
  bool card_bind_;
  core::int32 left_card_count_;
};

}   // namespace soul

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__SOUL__SOUL__ALTAR__ACTOR__H


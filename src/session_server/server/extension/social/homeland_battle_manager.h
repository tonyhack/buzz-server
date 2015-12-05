#ifndef __SESSION__SERVER__SOCIAL__HOMELAND__BATTLE__MANAGER__H
#define __SESSION__SERVER__SOCIAL__HOMELAND__BATTLE__MANAGER__H

#include <map>
#include <set>

#include "core/base/types.h"
#include "entity/homeland_types.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace social {

class SocialActor;

class HomelandBattleResult {
 public:
  HomelandBattleResult();
  ~HomelandBattleResult();

  entity::HomelandBattleResultType::type result_;
  bool commander_battle_attacker_win_;
  bool soul_battle_attacker_win_;
  core::int32 attacker_soul_alive_;
  core::int32 defender_soul_alive_;
};

class HomelandBattleManager : public global::SingletonFactory<HomelandBattleManager> {
 private:
  friend class global::SingletonFactory<HomelandBattleManager>;

  struct LadderEntry {
    core::uint64 actor_id_;
    core::int32 level_;
    core::uint32 random_perturb_;
  };

  struct LadderEntryLess {
    bool operator()(const LadderEntry *lhs, const LadderEntry *rhs) const {
      if (lhs->level_ != rhs->level_) {
        return lhs->level_ < rhs->level_;
      }
      if (lhs->random_perturb_ != rhs->random_perturb_) {
        return lhs->random_perturb_ < rhs->random_perturb_;
      }
      return lhs->actor_id_ < rhs->actor_id_;
    }
  };

  typedef std::map<core::uint64, LadderEntry> LadderEntryMap;
  typedef std::set<LadderEntry *, LadderEntryLess> LadderEntrySet;

 public:
  bool Initialize();
  void Finalize();

  void AddToBattleLadder(SocialActor *social_actor);
  void RemoveFromBattleLadder(core::uint64 actor_id);
  core::uint64 GetBattleTarget(core::uint64 actor_id);

  bool DoBattle(SocialActor *attacker, SocialActor *defender,
      HomelandBattleResult *battle_result, bool synchronize = true);

 private:
  HomelandBattleManager() {}
  ~HomelandBattleManager() {}

  LadderEntryMap ladder_;
  LadderEntrySet ladder_index_;
};

}  // namespace social

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SOCIAL__HOMELAND__BATTLE__MANAGER__H


#ifndef __SESSION__SERVER__SOCIAL__SOCIAL__ACTOR__MANAGER__H
#define __SESSION__SERVER__SOCIAL__SOCIAL__ACTOR__MANAGER__H

#include <cstddef>
#include <deque>
#include <set>
#include <vector>
#include <ext/hash_map>
#include <boost/function.hpp> 

#include "core/timer_handler.h"
#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace social {

class SocialActor;

class SocialActorManager : public global::SingletonFactory<SocialActorManager>,
                           public core::TimerHandler {
  friend class global::SingletonFactory<SocialActorManager>;
  enum { TIMER_ID_SAVE = 0 };

 public:
  typedef __gnu_cxx::hash_map<core::uint64, SocialActor *> ActorHashmap;
  typedef boost::function<void ()> OfflineTask;
  typedef __gnu_cxx::hash_map<core::uint64, OfflineTask> OfflineTaskHashmap;
  typedef std::deque<core::uint64> ActorIDDeque;
  typedef std::set<core::uint64> ActorIDSet;
  typedef std::vector<ActorIDSet> ActorIDSets;

  bool Initialize();
  void Finalize();

  SocialActor *Get(core::uint64 id);
  bool Add(SocialActor *actor);
  SocialActor *Remove(core::uint64 id);

  SocialActor *GetFromCache(core::uint64 id);
  bool AddToCache(SocialActor *actor);
  SocialActor *RemoveFromCache(core::uint64 id);

  SocialActor *GetFromAll(core::uint64 id);

  // 加载离线玩家
  void OfflineLoad(core::uint64 id, OfflineTask offline_task = NULL);
  OfflineTask RemoveOfflineTask(core::uint64 offline_task_id);

  // 加入待保存队列
  void AddToPendingSaveQueue(core::uint64 id);

  // 取附近等级的玩家
  const ActorIDSet *GetActorsNearLevel(core::int32 level);

  // 玩家升级事件处理
  void OnActorLevelup(core::uint64 id, core::int32 old_level, core::int32 new_level);

  // 每日清理
  void DailyClean();

  virtual void OnTimer(core::uint32 id);

 private:
  SocialActorManager() {}
  ~SocialActorManager() {}

  void PendingSaveActor();
  void AddToLevelPool(core::uint64 id);
  void RemoveFromLevelPool(core::uint64 id);

  ActorHashmap actors_;
  ActorHashmap cached_actors_;
  core::uint64 max_offline_task_id_;
  OfflineTaskHashmap offline_tasks_;
  ActorIDDeque pending_save_queue_;
  ActorIDSets real_time_level_actors_;
};

}  // namespace social

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SOCIAL__SOCIAL__ACTOR__MANAGER__H


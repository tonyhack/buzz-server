#ifndef __SESSION__SERVER__AUCTION__AUCTION__MANAGER__H
#define __SESSION__SERVER__AUCTION__AUCTION__MANAGER__H

#include <cstddef>
#include <ext/hash_map>
#include <boost/function.hpp>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace auction {

class AuctionActor;

class AuctionActorManager : public global::SingletonFactory<AuctionActorManager> {
  friend class global::SingletonFactory<AuctionActorManager>;

 public:
  typedef __gnu_cxx::hash_map<core::uint64, AuctionActor *> ActorHashmap;
  typedef boost::function<void ()> OfflineTask;
  typedef __gnu_cxx::hash_map<core::uint64, OfflineTask> OfflineTaskHashmap;

  bool Initialize();
  void Finalize();

  AuctionActor *Get(core::uint64 id);
  bool Add(AuctionActor *actor);
  AuctionActor *Remove(core::uint64 id);

  AuctionActor *GetFromCache(core::uint64 id);
  bool AddToCache(AuctionActor *actor);
  AuctionActor *RemoveFromCache(core::uint64 id);

  AuctionActor *GetFromAll(core::uint64 id);

  // 加载离线玩家
  void OfflineLoad(core::uint64 id, OfflineTask offline_task = NULL);
  OfflineTask RemoveOfflineTask(core::uint64 offline_task_id);
  // 保存缓存到数据库, 并且从缓存中移除
  void SaveAndRemoveFromCache(core::uint64 id);

 private:
  AuctionActorManager() {}
  ~AuctionActorManager() {}

  ActorHashmap actors_;
  ActorHashmap cached_actors_;
  core::uint64 max_offline_task_id_;
  OfflineTaskHashmap offline_tasks_;
};

}  // namespace auction

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__AUCTION__AUCTION__MANAGER__H


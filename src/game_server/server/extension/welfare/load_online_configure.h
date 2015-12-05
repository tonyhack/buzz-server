#ifndef __GAME__SERVER__WELFARE__LOAD___ONLINE__CONFIGURE__H
#define __GAME__SERVER__WELFARE__LOAD___ONLINE__CONFIGURE__H

#include <map>
#include <vector>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "game_server/server/extension_manager.h"
#include "game_server/server/extension/welfare/load_task_configure.h"

namespace game {

namespace server {

namespace welfare {

class OnlineCell {
 public:
  core::int32 id_;
  core::int32 online_time_;
  Award award_[5];
  std::vector<core::int32> rands_;
  core::int32 rand_result_;       // 随机出结果，临时存储
};

class LoadOnlineConfigure : public core::Noncopyable {
  typedef std::map<core::int32, OnlineCell> OnlineMap;
 public:
  LoadOnlineConfigure();
  ~LoadOnlineConfigure();

  bool LoadConfigure(const std::string &file);

  OnlineCell *RandCell(core::int32 id) {
    OnlineMap::iterator itr = this->onlines_.find(id);
    if(itr != this->onlines_.end()) {
      core::int32 odds = 0;
      itr->second.rand_result_ = -1;
      core::int32 reslut = ExtensionManager::GetInstance()->GetRandom().Random(100);
      for(size_t i = 0; i < itr->second.rands_.size(); ++i) {
        odds += itr->second.rands_[i];
        if(odds > reslut) {
          itr->second.rand_result_ = i;
          return &(itr->second);
        }
      }
    }
    return NULL;
  }

  core::int32 GetOnlineToPos(core::int32 online_time) {
    OnlineMap::const_iterator itr = this->onlines_.find(online_time);
    if(itr != this->onlines_.end()) {
      return itr->second.id_;
    }
    return -1;
  }

 private:
  OnlineMap onlines_;
};

}  // namespace welfare

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__WELFARE__LOAD___ONLINE__CONFIGURE__H


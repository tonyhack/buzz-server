//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-04-10 00:29:41.
// File name: load_progress.h
//
// Description:
// Define class LoadProgress.
//

#ifndef __GAME__SERVER__LOAD__PROGRESS__H
#define __GAME__SERVER__LOAD__PROGRESS__H

#include "core/base/noncopyable.h"
#include "game_server/server/extension_manager.h"

namespace game {

namespace server {

class LoadProgress : public core::Noncopyable {
 public:
  LoadProgress() {}
  ~LoadProgress() {}

  bool Initialize() {
    size_t pos = ExtensionType::MIN;
    for(; pos < ExtensionType::MAX; ++pos) {
      this->progress_[pos] = false;
    }
    return true;
  }

  void Finalize() {}

  bool LoadFinish() const {
    size_t pos = ExtensionType::MIN;
    for(; pos < ExtensionType::MAX; ++pos) {
      if(this->progress_[pos] == false) {
        return false;
      }
    }
    return true;
  }

  bool CheckExtensionFinish(ExtensionType::type type) {
    if(type < ExtensionType::MAX) {
      return this->progress_[type];
    }
    return false;
  }

  void LoadExtensionFinish(ExtensionType::type type) {
    if(type < ExtensionType::MAX) {
      this->progress_[type] = true;
    }
  }

 private:
  // 加载进度
  bool progress_[ExtensionType::MAX];
};

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__LOAD__PROGRESS__H


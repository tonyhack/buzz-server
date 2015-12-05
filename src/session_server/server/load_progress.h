//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-07-25 16:46:41.
// File name: load_progress.h
//
// Description:
// Define class LoadProgress.
//

#ifndef __SESSION__SERVER__LOAD__PROGRESS__H
#define __SESSION__SERVER__LOAD__PROGRESS__H

#include "core/base/noncopyable.h"
#include "session_server/server/extension_types.h"

namespace session {

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

  void LoadExtensionFinish(ExtensionType::type type) {
    if(type < ExtensionType::MAX) {
      this->progress_[type] = true;
    }
  }

 private:
  // 加载进度
  bool progress_[ExtensionType::MAX];
};

}  // naemspace server

}  // namespace session

#endif  // __SESSION__SERVER__LOAD__PROGRESS__H


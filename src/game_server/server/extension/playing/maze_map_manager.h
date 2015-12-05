#ifndef __GAME__SERVER__PLAYGING__MAZE__MAP__MANAGER__H
#define __GAME__SERVER__PLAYGING__MAZE__MAP__MANAGER__H

#include <ctime>
#include <vector>

#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace session { namespace protocol {
class MessageMazeMapInfoSynchronize;
}}

namespace game {

namespace server {

namespace playing {

class MazeMapManager : public global::SingletonFactory<MazeMapManager> {
  friend class global::SingletonFactory<MazeMapManager>;

 public:
  typedef std::vector<core::int32> MazeGridVector;
  typedef std::vector<MazeGridVector> MazeMapVector;

  bool Initialize();
  void Finalize();
  void Update(const session::protocol::MessageMazeMapInfoSynchronize &sync);

  bool CheckReady() const { return this->maps_.empty() == false; }
  time_t GetLastUpdateTime() const { return this->last_update_time_; }
  const MazeGridVector *GetMap(core::int32 floor) const;

 private:
  MazeMapManager();
  ~MazeMapManager();

  MazeMapVector maps_;
  time_t last_update_time_;
};

}  // namespace playing

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__PLAYGING__MAZE__MAP__MANAGER__H


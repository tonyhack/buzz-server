#ifndef __SESSION__SERVER__PLAYING__MAZE__MAP__MANAGER__H
#define __SESSION__SERVER__PLAYING__MAZE__MAP__MANAGER__H

#include <ctime>
#include <string>
#include <vector>

#include "core/timer_handler.h"
#include "core/base/types.h"
#include "global/singleton_factory.h"

namespace session { namespace protocol {
class MessageMazeMapInfoSynchronize;
}}

namespace session {

namespace server {

namespace playing {

class MazeMapManager : public global::SingletonFactory<MazeMapManager>,
                       public core::TimerHandler {
  friend class global::SingletonFactory<MazeMapManager>;
  enum {TIMER_ID_RESET_MAP = 0 };
 
 public:
  typedef std::vector<core::int32> MazeGridVector;
  typedef std::vector<MazeGridVector> MazeMapVector;

  bool Initialize();
  void Finalize();
  void ExportMapInfo(session::protocol::MessageMazeMapInfoSynchronize &sync);

  // 定时器
  virtual void OnTimer(core::uint32 id);

 private:
  MazeMapManager();
  ~MazeMapManager();

  bool Deserialize(const std::string &input);
  void Serialize(std::string *ouput);
  void AddResetMazeTimer(time_t now = 0);
  void RemoveResetMazeTimer();
  void ResetMazeMaps(bool startup = false);

  MazeMapVector maps_;
  time_t last_update_time_;
};

}  // namespace playing

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__PLAYING__MAZE__MAP__MANAGER__H


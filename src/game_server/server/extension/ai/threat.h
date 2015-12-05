//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-09-23 18:16:25.
// File name: threat.h
//
// Description:
//
#ifndef __GAME__SERVER__AI__THREAT__H
#define __GAME__SERVER__AI__THREAT__H

#include <cstddef>
#include <map>

#include "core/base/types.h"
#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace ai {

class BlackBoard;

class Threat : public core::Noncopyable {
  typedef std::map<core::uint64, core::int32> ThreatMap;

 public:
  Threat();
  ~Threat();

  bool Initialize(BlackBoard *black_board);
  void Finalize();
  
  bool Add(core::uint64 id, core::int32 value);

  bool Remove(core::uint64 id);

  void RemoveAll();

  core::int32 GetThreat(const core::uint64 id) const;
  
  core::uint64 GetMaxThreatActor() const;

  inline size_t GetSize() const {
    return this->threat_map_.size();
  }

 private:
  ThreatMap threat_map_;
  core::int32 max_hate_;
  core::uint64 max_hate_id_;

  BlackBoard *black_board_;
};

} // namespace ai

} // namespace server

} // namespace game

#endif // __GAME__SERVER__AI__THREAT__H


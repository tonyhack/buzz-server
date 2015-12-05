#ifndef __SESSION__SERVER__GUILD__GUILD__EVENT__H
#define __SESSION__SERVER__GUILD__GUILD__EVENT__H

#include <ctime>
#include <string>
#include <vector>

#include "core/base/types.h"

namespace session {

namespace server {

namespace guild {

class GuildEvent {
 public:
  typedef std::vector<std::string> EventVarVector;

  GuildEvent();
  GuildEvent(core::int32 type);
  ~GuildEvent();

  bool Initialize(const std::string &data);

  core::int32 GetType() const { return this->type_; }
  const std::string &GetTimestamp() const { return this->timestamp_; }
  const EventVarVector &GetEventVars() const { return this->event_vars_; }

  void PushEventVar(const std::string &event_var);
  void PushEventVar(core::uint64 event_var);
  void PushEventVar(core::int32 event_var);
  void TranslateEventVar(std::vector<std::string> *result) const;

 private:
  core::int32 type_;
  std::string timestamp_;
  EventVarVector event_vars_;
};

}  // namespace guild

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__GUILD__GUILD__EVENT__H


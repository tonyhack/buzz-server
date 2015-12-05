#ifndef __SESSION__SERVER__SOCIAL__HOMELAND__EVENT__H
#define __SESSION__SERVER__SOCIAL__HOMELAND__EVENT__H

#include <ctime>
#include <string>
#include <vector>

#include "core/base/types.h"

namespace session {

namespace server {

namespace social {

class HomelandEvent {
 public:
  typedef std::vector<std::string> EventVarVector;

  HomelandEvent();
  HomelandEvent(core::int32 type);
  ~HomelandEvent();

  bool Initialize(const std::string &data);

  core::int32 GetType() const { return this->type_; }
  const std::string &GetTimestamp() const { return this->timestamp_; }
  const EventVarVector &GetEventVars() const { return this->event_vars_; }

  void PushEventVar(const std::string &event_var);
  void PushEventVar(core::uint64 event_var);
  void PushEventVar(core::uint32 event_var);
  void PushEventVar(core::int32 event_var);
  void TranslateEventVar(std::vector<std::string> *result) const;

 private:
  core::int32 type_;
  std::string timestamp_;
  EventVarVector event_vars_;
};

}  // namespace social

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__SOCIAL__HOMELAND__EVENT__H


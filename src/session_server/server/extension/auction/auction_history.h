#ifndef __SESSION__SERVER__AUCTION__AUCTION__HISTORY__H
#define __SESSION__SERVER__AUCTION__AUCTION__HISTORY__H

#include <ctime>
#include <string>
#include <vector>

#include "core/base/types.h"

namespace session {

namespace server {

namespace auction {

class AuctionHistory {
 public:
  typedef std::vector<std::string> HistoryVarVector;

  AuctionHistory();
  AuctionHistory(core::int32 type);
  ~AuctionHistory();

  bool Initialize(const std::string &data);

  core::int32 GetType() const { return this->type_; }
  const std::string &GetTimestamp() const { return this->timestamp_; }
  const HistoryVarVector &GetHistoryVars() const { return this->history_vars_; }

  void PushHistoryVar(const std::string &event_var);
  void PushHistoryVar(core::uint64 event_var);
  void PushHistoryVar(core::uint32 event_var);
  void PushHistoryVar(core::int32 event_var);
  void TranslateHistoryVar(std::vector<std::string> *result) const;

 private:
  core::int32 type_;
  std::string timestamp_;
  HistoryVarVector history_vars_;
};

}  // namespace auction

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__AUCTION__AUCTION__HISTORY__H


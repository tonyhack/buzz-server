#ifndef __GLOBAL__CONFIGURE__AUCTION__CONFIGURE__H
#define __GLOBAL__CONFIGURE__AUCTION__CONFIGURE__H

#include <string>
#include <ext/hash_map>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace global {

namespace configure {

class AuctionTimeCell {
 public:
  // 拍卖时间ID
  core::int32 id_;

  // 拍卖时间(秒)
  core::int32 time_;

  // 委托费
  core::int32 fee_;
};

class AuctionConfigure : public core::Noncopyable {
 public:
  typedef __gnu_cxx::hash_map<core::int32, AuctionTimeCell> AuctionTimeHashmap;

  AuctionConfigure();
  ~AuctionConfigure();

  bool LoadConfigure(const std::string &file);

  // 拍卖时间
  const AuctionTimeCell *GetAuctionTime(core::int32 id) const;

 private:
  bool LoadAuctionTime(const std::string &file);

  AuctionTimeHashmap auction_times_;
};

}  // namespace configure

}  // namespace global

#endif  // __GLOBAL__CONFIGURE__AUCTION__CONFIGURE__H


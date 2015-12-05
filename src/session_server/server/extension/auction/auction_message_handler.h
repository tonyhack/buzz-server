#ifndef __SESSION__SERVER__AUCTION__AUCTION__MESSAGE__HANDLER__H
#define __SESSION__SERVER__AUCTION__AUCTION__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

namespace auction {

class AuctionMessageHandler : public core::Noncopyable {
 public:
  AuctionMessageHandler();
  ~AuctionMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 取交易信息请求
  void OnMessageAuctionQueryTransactionRequest(core::uint64 id, const char *data, size_t size);
  // 搜索道具请求
  void OnMessageAuctionSearchRequest(core::uint64 id, const char *data, size_t size);
  // 上架道具请求
  void OnMessageAuctionSellRequest(core::uint64 id, const char *data, size_t size);
  // 下架道具请求
  void OnMessageAuctionCancelSellRequest(core::uint64 id, const char *data, size_t size);
  // 竞拍道具请求
  void OnMessageAuctionBidRequest(core::uint64 id, const char *data, size_t size);
  // 一口价购买道具请求
  void OnMessageAuctionBuyoutRequest(core::uint64 id, const char *data, size_t size);
  // 提取(道具/交易金额)请求
  void OnMessageAuctionWithdrawRequest(core::uint64 id, const char *data, size_t size);
};

}  // namespace auction

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__AUCTION__AUCTION__MESSAGE__HANDLER__H


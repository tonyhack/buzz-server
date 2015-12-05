#ifndef __GAME__SERVER__AUCTION__SESSION__AUCTION__MESSAGE__HANDLER__H
#define __GAME__SERVER__AUCTION__SESSION__AUCTION__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"

namespace game {

namespace server {

namespace auction {

class SessionAuctionMessageHandler : public core::Noncopyable {
 public:
  SessionAuctionMessageHandler();
  ~SessionAuctionMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 上架道具请求
  void OnMessageAuctionSellRequest(const char *data, size_t size);
  // 竞拍道具请求
  void OnMessageAuctionBidRequest(const char *data, size_t size);
  // 一口价购买道具请求
  void OnMessageAuctionBuyoutRequest(const char *data, size_t size);
  // 提取道具请求
  void OnMessageAuctionWithdrawItemRequest(const char *data, size_t size);
  // 提取道具请求2
  void OnMessageAuctionWithdrawItemRequest2(const char *data, size_t size);
  // 提取交易金额请求
  void OnMessageAuctionWithdrawMoneyRequest(const char *data, size_t size);
  // 退还拍卖金请求
  void OnMessageAuctionGiveMoneyBackRequest(const char *data, size_t size);
};

}  // namespace auction

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__AUCTION__SESSION__AUCTION__MESSAGE__HANDLER__H


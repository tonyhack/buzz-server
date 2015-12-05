#ifndef __SESSION__SERVER__AUCTION__GAME__AUCTION__MESSAGE__HANDLER__H
#define __SESSION__SERVER__AUCTION__GAME__AUCTION__MESSAGE__HANDLER__H

#include <cstddef>

#include "core/base/noncopyable.h"
#include "core/base/types.h"

namespace session {

namespace server {

class SessionTerminal;

namespace auction {

class GameAuctionMessageHandler : public core::Noncopyable {
 public:
  GameAuctionMessageHandler();
  ~GameAuctionMessageHandler();

  bool Initialize();
  void Finalize();

 private:
  // 上架道具回复
  void OnMessageAuctionSellResponse(SessionTerminal *terminal,
      const char *data, size_t size);
  // 竞拍道具回复
  void OnMessageAuctionBidResponse(SessionTerminal *terminal,
      const char *data, size_t size);
  // 一口价购买道具回复
  void OnMessageAuctionBuyoutResponse(SessionTerminal *terminal,
      const char *data, size_t size);
  // 提取道具回复
  void OnMessageAuctionWithdrawItemResponse(SessionTerminal *terminal,
      const char *data, size_t size);
};

}  // namespace auction

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__AUCTION__GAME__AUCTION__MESSAGE__HANDLER__H


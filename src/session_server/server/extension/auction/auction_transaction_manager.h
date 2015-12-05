#ifndef __SESSION__SERVER__AUCTION__AUCTION__TRANSACTION__MANAGER__H
#define __SESSION__SERVER__AUCTION__AUCTION__TRANSACTION__MANAGER__H

#include <map>
#include <set>
#include <string>
#include <vector>
#include <mysql++/mysql++.h>

#include "core/base/types.h"
#include "entity/auction_types.h"
#include "global/singleton_factory.h"

namespace session {

namespace server {

namespace auction {

class AuctionActor;
class AuctionTransaction;
 
// 索引比较函数
struct AuctionTransactionLessItemTemplateID {
  bool operator()(const AuctionTransaction *lhs, const AuctionTransaction *rhs) const;
};
struct AuctionTransactionLessItemNumebr {
  bool operator()(const AuctionTransaction *lhs, const AuctionTransaction *rhs) const;
};
struct AuctionTransactionLessEndBidTime {
  bool operator()(const AuctionTransaction *lhs, const AuctionTransaction *rhs) const;
};
struct AuctionTransactionLessBidPrice {
  bool operator()(const AuctionTransaction *lhs, const AuctionTransaction *rhs) const;
};


class AuctionTransactionManager : public global::SingletonFactory<AuctionTransactionManager> {
  friend class global::SingletonFactory<AuctionTransactionManager>;

 public:
  typedef std::map<std::string, AuctionTransaction *> AuctionTransactionMap;
  typedef std::set<AuctionTransaction *, AuctionTransactionLessItemTemplateID> ItemTemplateIDIndex;
  typedef std::set<AuctionTransaction *, AuctionTransactionLessItemNumebr> ItemNumberIndex;
  typedef std::set<AuctionTransaction *, AuctionTransactionLessEndBidTime> EndBidTimeIndex;
  typedef std::set<AuctionTransaction *, AuctionTransactionLessBidPrice> BidPriceIndex;
  typedef std::map<core::int32, ItemTemplateIDIndex> ItemTemplateIDIndexMap;
  typedef std::map<core::int32, ItemNumberIndex> ItemNumberIndexMap;
  typedef std::map<core::int32, EndBidTimeIndex> EndBidTimeIndexMap;
  typedef std::map<core::int32, BidPriceIndex> BidPriceIndexMap;

  bool Initialize();
  void Finalize();

  void BuildIndex();
  bool CheckIndexBuilt() const { return this->index_built_; }

  AuctionTransaction *Get(const std::string &id);
  bool Add(AuctionTransaction *transaction);
  AuctionTransaction *Remove(const std::string &id);

  // 搜索交易
  void SearchTransaction(core::uint32 item_template_ids[], size_t number,
      entity::AuctionSearchSortType::type sort_type, core::int32 page,
      std::vector<std::string> &result, core::int32 &total_page);
  void SearchTransaction(core::int32 item_type,
      entity::AuctionSearchSortType::type sort_type, core::int32 page,
      std::vector<std::string> &result, core::int32 &total_page);
  // 创建交易
  bool CreateTransaction(AuctionActor *auction_actor,
      core::uint32 item_template_id, core::int32 item_number,
      const std::string &item_extra_info, core::int32 auction_time_id,
      core::int32 start_bid_price, core::int32 buyout_price,
      bool synchronize = true);
  // 下架交易
  void CancelTransaction(AuctionActor *auction_actor,
      AuctionTransaction *transaction, bool synchronize = true);
  // 重新上架交易
  bool RenewTransaction(AuctionActor *auction_actor,
      AuctionTransaction *transaction, bool synchronize = true);
  // 竞价
  bool BidTransaction(AuctionActor *auction_actor,
      AuctionTransaction *transaction, core::int32 bid_price,
      bool synchronize = true);
  // 一口价
  bool BuyoutTransaction(AuctionActor *auction_actor,
      AuctionTransaction *transaction, core::int32 buyout_price,
      bool synchronize = true);
  // 结束交易
  void FinishTransaction(AuctionTransaction *transaction,
      bool buyout = false, bool synchronize = true);
  // 卖家提取交易金额
  void OwnerWithdrawMoney(AuctionActor *auction_actor,
      AuctionTransaction *transaction, bool synchronize = true);
  // 卖家提取道具
  void OwnerWithdrawItem(AuctionActor *auction_actor,
      AuctionTransaction *transaction, bool synchronize = true);
  // 买家提取交易金额
  void BuyerWithdrawMoney(AuctionActor *auction_actor,
      const std::string &transaction_id, bool synchronize = true);
  // 买家提取道具
  void BuyerWithdrawItem(AuctionActor *auction_actor,
      AuctionTransaction *transaction, bool synchronize = true);

 private:
  AuctionTransactionManager() {}
  ~AuctionTransactionManager() {}

  bool LoadAuctionTransactions(mysqlpp::Connection &connection);
  void AddToIndex(AuctionTransaction *transaction);
  void RemoveFromIndex(AuctionTransaction *transaction);
  // 销毁交易
  void DestroyTransaction(const std::string &transaction_id);

  // 交易
  AuctionTransactionMap transactions_;
  core::uint32 max_transaction_id_offset_;
  // 交易索引
  ItemTemplateIDIndexMap item_template_id_indexes_;
  ItemNumberIndexMap item_number_indexes_;
  EndBidTimeIndexMap end_bid_time_indexes_;
  BidPriceIndexMap bid_price_indexes_;
  bool index_built_;
};

}  // namespace auction

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__AUCTION__AUCTION__TRANSACTION__MANAGER__H


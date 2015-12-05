namespace cpp database.protocol

enum WelfareField {
  TITLE_ID,
  DAILY_CHECKINS,
  DAILY_AWARD,
  GENERAL_CHECKINS,
  VIP_CHECKINS,
  LAST_MONTH_RESOURCE,
  CURRENT_MONTH_RESOURCE,
  LAST_MONTH_AWARD_RESOURCE,
  DAILY_TIME,
  TOTAL_TIME,
  REPLENISH_CHECKINS_NUM,
  ONLINE_AWARD_ITEMS,
  RESPENISH_DAYS,

  MAX,
}

const list<string> kWelfareFieldStr = [
      "@ret_title_id",
      "@ret_daily_checkins",
      "@ret_daily_award",
      "@ret_general_checkins",
      "@ret_vip_checkins",
      "@ret_last_month_resource",
      "@ret_current_month_resource",
      "@ret_last_month_award_resource",
      "@ret_daily_time",
      "@ret_total_time",
      "@ret_replenish_checkins_num",
      "@ret_online_award_items",
      "@ret_respenish_days",
]


enum WelfareLoginReslut {
  SUCCESS = 0,
  ERROR_INEXIST,
  ERROR_UNKNOWN,

  MAX,
}

struct StorageWelfareField {
   1 : required i32 title_id_;
   2 : required i32 daily_checkins_;
   3 : required bool daily_award_;
   4 : required i32 general_checkins_;
   5 : required i32 vip_checkins_;
   6 : required i32 last_month_resource_;
   7 : required i32 current_month_resource_;
   8 : required bool last_month_award_resource_;
   9 : required i32 daily_time_;
   10 : required i32 total_time_;
   11 : required i32 replenish_checkins_num_;
   12 : required string online_award_items_;
   13 : required string respenish_days_;
}

// type: STORAGE_WELFARE_LOAD
struct StorageWelfareLoadRequest {
  1 : required i64 actor_;
}

struct StorageWelfareLoadResponse {
  1 : required i64 actor_;
  2 : required StorageWelfareField field_;
  3 : required WelfareLoginReslut result_;
}


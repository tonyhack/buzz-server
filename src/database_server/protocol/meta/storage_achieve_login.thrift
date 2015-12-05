namespace cpp database.protocol

enum AchieveField {
  ACHIEVE_ID,
  FLAG,
  CONDITION1_PARAM1,
  CONDITION1_PARAM2,
  CONDITION1_VALUE,
  CONDITION2_PARAM1,
  CONDITION2_PARAM2,
  CONDITION2_VALUE,
  FINISH_TIME,
  MAX,
}

const list<string> kAchieveFieldStr = [
    "@ret_achieve_id",
    "@ret_flag",
    "@ret_condition1_param1",
    "@ret_condition1_param2",
    "@ret_condition1_value",
    "@ret_condition2_param1",
    "@ret_condition2_param2",
    "@ret_condition2_value",
    "@ret_finish_time",
]


enum AchieveLoginResult {
  SUCCESS = 0,
  ERROR_INEXIST,
  ERROR_UNKNOWN,

  MAX,
}

struct StorageAchieveField {
  1 : required i32 achieve_id_;
  2 : required i32 condition1_param1_;
  3 : required i32 condition1_param2_;
  4 : required i32 condition1_value_;
  5 : required i32 condition2_param1_;
  6 : required i32 condition2_param2_;
  7 : required i32 condition2_value_;
  8 : required i64 finish_time_;
}

struct StorageAchieveFinishField {
  1 : required i32 achieve_id_;
  2 : required i64 finish_time_;
}


// type: STORAGE_ACHIEVE_LOGIN
struct StorageAchieveLoginRequest {
  1 : required i64 actor_;
}

struct StorageAchieveLoginResponse {
  1 : required i64 actor_;
  2 : required list<StorageAchieveField>        achieves_;
  3 : required list<StorageAchieveFinishField>  finish_achieves_;
  4 : required AchieveLoginResult result_;
}

enum AchieveFlag {
  DEL = 0,
  ACTIVE = 1,
  FINISH = 2,
}

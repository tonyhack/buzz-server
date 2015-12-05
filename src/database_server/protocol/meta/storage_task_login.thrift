namespace cpp database.protocol

enum TaskField {
  TASK = 0,
  FLAG,
  COND_TYPE1,
  VAR1,
  VALUE1,
  COND_TYPE2,
  VAR2,
  VALUE2,
  COND_TYPE3,
  VAR3,
  VALUE3,
  TASK_TYPE,

  MAX,
}

const list<string> kTaskFieldStr = [
  "task",                                           // 任务ID
  "flag",                                           // 任务标志(0:不存在，1:正在进行, 2:完成)
  "cond_type1",                                     // 条件类型1
  "var1",                                           // 变量1
  "value1",                                         // 值1
  "cond_type2",                                     // 条件类型2
  "var2",                                           // 变量2
  "value2",                                         // 值2
  "cond_type3",                                     // 条件类型3
  "var3",                                           // 变量3
  "value3",                                         // 值3
  "task_type",                                      // 任务类型
]

enum TaskLoginResult {
  SUCCESS = 0,
  ERROR_UNKNOWN,
}

// 任务标志
enum TaskFlag {
  NOT_EXIST = 0,                                    // 不存在
  ACTIVE,                                           // 活动的
  FINISHED,                                         // 完成的

  MAX,
}

struct StorageTaskField {
  1 : required i32 task_;
  2 : required byte flag_;
  3 : required byte cond_type1_;
  4 : required i32 var1_;
  5 : required i32 value1_;
  6 : required byte cond_type2_;
  7 : required i32 var2_;
  8 : required i32 value2_;
  9 : required byte cond_type3_;
  10: required i32 var3_;
  11: required i32 value3_;
  12: required byte task_type_;
}


// 完成任务列表
struct StorageTaskFinish {
 1 : required i32 task_id_;                         // 任务ID
 2 : required i32 type_;                            // 任务类型 
}

// type: STORAGE_TASK_LOGIN
struct StorageTaskLoginRequest {
  1 : required i64 actor_id_;                       // 玩家ID
}

struct StorageTaskLoginResponse {
  1 : required TaskLoginResult result_;             // 结果
  2 : required list<StorageTaskField> actives_;     // 正在做的任务集
  3 : required list<StorageTaskFinish> finishes_;   // 完成过的任务集
}


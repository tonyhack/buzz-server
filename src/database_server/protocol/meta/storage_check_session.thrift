namespace cpp database.protocol

// type: STORAGE_CHECK_SESSION
struct StorageCheckSessionRequest {
  1 : required i64 id_;                   // 角色ID
  2 : required string session_;           // 会话
  3 : required byte session_status_;      // 期望的会话状态值
  4 : required i64 effective_time_;       // 会话有效时间
  5 : required i64 sequence_;             // 请求序列号
}
struct StorageCheckSessionResponse {
  1 : required bool pass_;                // 是否通过检测
  2 : required bool online_;              // 是否当前在线
}


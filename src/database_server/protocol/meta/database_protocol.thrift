include "server_type.thrift"
include "database_storage.thrift"

namespace cpp database.protocol

enum MessageType {
  MESSAGE_MIN = 0,

  MESSAGE_LOGIN_REQUEST = 0,
  MESSAGE_LOGIN_RESPONSE,

  MESSAGE_LOGOUT_REQUEST,
  MESSAGE_LOGOUT_RESPONSE,

  MESSAGE_STORAGE_REQUEST,
  MESSAGE_STORAGE_RESPONSE,

  MESSAGE_MAX,
}

enum ResponseType {
  SUCCESS = 0,
  ERROR_MESSAGE,
  ERROR_DISCONNECTED,
  ERROR_UNKNOWN,

  MAX,
}


struct Message {
  1 : required MessageType type_;
  2 : required binary message_;
}



// type: MESSAGE_LOGIN_REQUEST
// dir: other->database
struct MessageLoginRequest {
  1 : required server_type.ServerSignature signature_;
}

// type: MESSAGE_LOGIN_RESPONSE
// dir: database->other
struct MessageLoginResponse {
  1 : required bool logined_;
}



// type: MESSAGE_LOGOUT_REQUEST
// dir: other->database
struct MessageLogoutRequest {
}

// type: MESSAGE_LOGOUT_RESPONSE
// dir: database->other
struct MessageLogoutResponse {
  1 : required bool logouted_;
}



// type: MESSAGE_STORAGE_REQUEST
// dir: other->database
struct MessageStorageRequest {
  1 : required database_storage.StorageType type_;
  2 : required binary message_;
  3 : required i64 channel_;
}

// type: MESSAGE_STORAGE_RESPONSE
// dir: database->other
struct MessageStorageResponse {
  1 : required MessageStorageRequest request_;
  2 : required ResponseType result_;
  3 : required binary response_;
}


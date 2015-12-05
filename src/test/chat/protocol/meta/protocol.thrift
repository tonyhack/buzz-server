namespace cpp test.chat.protocol

enum MessageType {
  MESSAGE_LOGIN_REQUEST,
  MESSAGE_LOGIN_RESPONSE,
  MESSAGE_LOGIN_BROADCAST,

  MESSAGE_LOGOUT_REQUEST,
  MESSAGE_LOGOUT_RESPONSE,
  MESSAGE_LOGOUT_BROADCAST,

  MESSAGE_CHAT,

  MESSAGE_ERROR,
}


struct Message {
  1 : required MessageType type_;
  2 : optional binary message_;
}



// MESSAGE_LOGIN_REQUEST
struct MessageLoginRequest {
  1 : required string name_;
}

// MESSAGE_LOGIN_RESPONSE
struct MessageLoginResponse {
  1 : required bool success_;
}

// MESSAGE_LOGIN_BROADCAST
struct MessageLoginBroadcast {
  1 : required string name_;
}



// MESSAGE_LOGOUT_REQUEST
struct MessageLogoutRequest {
}

// MESSAGE_LOGOUT_RESPONSE
struct MessageLogoutResponse {
  1 : required bool success_;
}

// MESSAGE_LOGOUT_BROADCAST
struct MessageLogoutBroadcast {
  1 : required string name_;
}



enum ChatType {
  CHAT_PRIVATE,
  CHAT_BROADCAST,
}

// MESSAGE_CHAT
struct MessageChat {
  1 : required ChatType type_;
  2 : required string from_;
  3 : optional string to_;
  4 : required string contect_;
}


// MESSAGE_ERROR
struct MessageError {
  1 : required string contect_;
}


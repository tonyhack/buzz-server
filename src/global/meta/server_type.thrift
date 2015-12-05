namespace cpp global.protocol

enum ServerType {
  DATABASE_SERVER = 0,
  GAME_SERVER,
  GATEWAY_SERVER,
  SCHEDULE_SERVER,
  SESSION_SERVER,

  MAX,
}

struct ServerSignature {
  1 : required ServerType type_;
  2 : required i32 id_;
  3 : required string host_;
  4 : required i16 port_;
}


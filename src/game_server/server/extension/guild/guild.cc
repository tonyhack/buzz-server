#include "game_server/server/extension/guild/guild.h"

#include "session_server/protocol/session_game_protocol_types.h"
#include "session_server/protocol/game_guild_protocol_types.h"

namespace game {

namespace server {

namespace guild {

Guild::Guild() : id_(0), level_(0), 
  light_attribute_(0), dark_attribute_(0), holy_attribute_(0),
  shop_level_(0), college_level_(0), barrack_level_(0) {}

Guild::~Guild() {}

bool Guild::Initialize(const session::protocol::GuildData &data) {
  this->id_ = data.id_;
  this->name_ = data.name_;
  this->level_ = data.level_;
  this->light_attribute_ = data.light_attribute_;
  this->dark_attribute_ = data.dark_attribute_;
  this->holy_attribute_ = data.holy_attribute_;
  this->shop_level_ = data.shop_level_;
  this->college_level_ = data.college_level_;
  this->barrack_level_ = data.barrack_level_;

  return true;
}

void Guild::Finalize() {
  this->barrack_level_ = 0;
  this->college_level_ = 0;
  this->shop_level_ = 0;
  this->holy_attribute_ = 0;
  this->dark_attribute_ = 0;
  this->light_attribute_ = 0;
  this->level_ = 0;
  this->name_.clear();
  this->id_ = 0;
}

}  // namespace guild

}  // namespace server

}  // namespace game


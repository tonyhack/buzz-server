//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-09-25 17:14:00.
// File name: variables.cc
//
// Description:
// Define class Variables.
//

#include "game_server/server/extension/ai/variables.h"

namespace game {

namespace server {

namespace ai {

Variables::Variables() {}
Variables::~Variables() {}

bool Variables::Initialize() {
  return true;
}

void Variables::Finalize() {
  this->variables_.clear();
}

void Variables::Set(const std::string &key, core::int64 value) {
  VariableHashmap::iterator iterator = this->variables_.find(key);
  if(iterator == this->variables_.end()) {
    this->variables_.insert(std::make_pair(key, value));
  } else {
    iterator->second = value;
  }
}

core::int64 Variables::Get(const std::string &key) const {
  VariableHashmap::const_iterator iterator = this->variables_.find(key);
  if(iterator != this->variables_.end()) {
    return iterator->second;
  }
  return 0;
}

core::int64 Variables::Increase(const std::string &key, core::int64 increment) {
  VariableHashmap::iterator iterator = this->variables_.find(key);
  if(iterator == this->variables_.end()) {
    this->variables_.insert(std::make_pair(key, increment));
    return increment;
  } else {
    iterator->second += increment;
    return iterator->second;
  }
}

core::int64 Variables::Decrease(const std::string &key, core::int64 decrement) {
  VariableHashmap::iterator iterator = this->variables_.find(key);
  if(iterator == this->variables_.end()) {
    this->variables_.insert(std::make_pair(key, 0 - decrement));
    return 0 - decrement;
  } else {
    iterator->second -= decrement;
    return iterator->second;
  }
}

}  // namespace ai

}  // namespace server

}  // namespace game


//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 18:22:54.
// File name: packetizer_table.cc
//
// Description:
// Define class PacketizerTable.
//

#include "coresh/packetizer_table.h"

namespace coresh {

PacketizerTable PacketizerTableSingleton::table_;

PacketizerTable::PacketizerTable() {}

PacketizerTable::~PacketizerTable() {}

bool PacketizerTable::Insert(core::EventLoop *key, Packetizer *packetizer) {
  PacketizerMap::iterator iterator = this->packetizers_.find(key);
  if(iterator == this->packetizers_.end()) {
    this->packetizers_.insert(std::make_pair(key, packetizer));
    return true;
  }
  return false;
}

Packetizer *PacketizerTable::Remove(core::EventLoop *key) {
  Packetizer *packetizer = NULL;
  PacketizerMap::iterator iterator = this->packetizers_.find(key);
  if(iterator != this->packetizers_.end()) {
    packetizer = iterator->second;
    this->packetizers_.erase(iterator);
  }
  return packetizer;
}

Packetizer *PacketizerTable::GetPacketizer(core::EventLoop *key) {
  PacketizerMap::iterator iterator = this->packetizers_.find(key);
  if(iterator != this->packetizers_.end()) {
    return iterator->second;
  }
  return NULL;
}

}  // namespace coresh


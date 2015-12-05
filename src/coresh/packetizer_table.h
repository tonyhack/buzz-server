//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2012-12-27 18:20:56.
// File name: packetizer_table.h
//
// Description:
// Define class PacketizerTable.
//

#ifndef __CORESH__PACKETIZER__TALBE__H
#define __CORESH__PACKETIZER__TALBE__H

#include <map>

#include "core/event_loop.h"
#include "core/base/noncopyable.h"
#include "coresh/packetizer.h"

namespace coresh {

class PacketizerTable : public core::Noncopyable {
  typedef std::map<core::EventLoop *, Packetizer *> PacketizerMap;
 public:
  PacketizerTable();
  ~PacketizerTable();

  bool Insert(core::EventLoop *key, Packetizer *packetizer);
  Packetizer *Remove(core::EventLoop *key);

  Packetizer *GetPacketizer(core::EventLoop *key);

 private:
  PacketizerMap packetizers_;
};

class PacketizerTableSingleton : public core::Noncopyable {
 public:
  PacketizerTableSingleton() {}
  ~PacketizerTableSingleton() {}

  static inline PacketizerTable &GetInstance() {
    return PacketizerTableSingleton::table_;
  }

 private:
  static PacketizerTable table_;
};

}  // namespace coresh

#endif  // __CORESH__PACKETIZER__TALBE__H


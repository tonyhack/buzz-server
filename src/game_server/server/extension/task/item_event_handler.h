//
// Summary: buzz source code.
//
// Author: LuXuefeng.
// Email: lxf@gmail.com.
// Last modify: 2013-10-09 22:35:23.
// File name: item_event_handler.h
//
// Description:
//

#ifndef __GAME__SERVER__TASK__ITEM__EVENT__HANDLER__H
#define __GAME__SERVER__TASK__ITEM__EVENT__HANDLER__H

#include <stddef.h>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "coresh/communicator.h"

namespace game {

namespace server {

namespace task {

class ItemEventHandler : public core::Noncopyable {
 public:
  ItemEventHandler();
  ~ItemEventHandler();

  bool Initialize();
  void Finalize();

 private:

  // 道具使用
  void OnUseEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  // 装备洗练
  void OnRefineEvent(core::uint64 channel, core::uint8 channel_type,
      const void *message, size_t size);
  // 装备强化
  void OnStrengthEvent(core::uint64 channel, core::uint8 channel_type,
       const void *message, size_t size);
  // 宝石镶嵌
  void OnInlayEvent(core::uint64 channel, core::uint8 channel_type,
       const void *message, size_t size);
  // 占星
  void OnDivineGemEvent(core::uint64 channel, core::uint8 channel_type,
       const void *message, size_t size);
  // 占星
  void OnWearEvent(core::uint64 channel, core::uint8 channel_type,
       const void *message, size_t size);

  coresh::Communicator::Connection conn_item_use_;
  coresh::Communicator::Connection conn_equip_refine_;
  coresh::Communicator::Connection conn_equip_strength_;
  coresh::Communicator::Connection conn_equip_inlay_;
  coresh::Communicator::Connection conn_divine_gem_;
  coresh::Communicator::Connection conn_wear_equip_;
};

}  // namespace task

}  // namespace server

}  // namespace game

#endif  // __GAME__SERVER__TASK__ITEM__EVENT__HANDLER__H


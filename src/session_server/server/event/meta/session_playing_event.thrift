include "session_event.thrift"

namespace cpp session.server.event


// 副本结束事件
// type: EVENT_PLAYING_END
struct EventPlayingEnd {
  1 : required i64 playing_id_;                   // 副本ID
  2 : required i32 playing_template_id_;          // 副本模板ID
}


// 副本销毁事件
// type: EVENT_PLAYING_DESTROY
struct EventPlayingDestroy {
  1 : required i64 playing_id_;                   // 副本ID
  2 : required i32 playing_template_id_;          // 副本模板ID
}


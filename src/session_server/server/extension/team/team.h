//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2013-08-21 17:34:17.
// File name: team.h
//
// Description:
// Define class Team.
//

#ifndef __SESSION__SERVER__TEAM__TEAM__H
#define __SESSION__SERVER__TEAM__TEAM__H

#include <vector>

#include "core/base/noncopyable.h"
#include "core/base/types.h"
#include "gateway_server/protocol/gateway_protocol_types.h"
#include "global/logging.h"
#include "session_server/server/session_server.h"
#include "session_server/server/extension/team/team_actor.h"

namespace session {

namespace server {

namespace team {

class Team : public core::Noncopyable {
 public:
  typedef std::vector<TeamActor *> MemberVector;

  Team();
  ~Team();

  bool Initialize();
  void Finalize();

  // 增加/删除/获取 队员
  bool AddMember(TeamActor *member);
  bool RemoveMember(TeamActor *member);

  // 解散
  bool RemoveAll();

  // 设置新队长
  bool SetNewCaptain(TeamActor *memeber);

  // 获取队伍人数
  inline size_t GetSize() const {
    return this->members_.size();
  }

  // 获取/设置 队伍ID
  inline core::uint64 GetID() const {
    return this->id_;
  }
  inline void SetID(core::uint64 id) {
    this->id_ = id;
  }

  inline bool CheckMemberFull() const {
    return this->GetSize() >= Team::kMaxSize_;
  }

  inline const MemberVector &GetMembers() const {
    return this->members_;
  }

  // 得到队员平均等级
  core::int32 GetAverageLevel();

  // 同步队伍数据到玩家
  void Synchronize(TeamActor *actor);

  // 同步组队BUFF
  void SynchronizeBuff();

  void BroadcastMessage(gateway::protocol::MessageType::type type,
      const char *message, size_t size);

  template <typename T>
  void BroadcastMessage(const T &message, gateway::protocol::MessageType::type type);

 private:
  // 队伍ID
  core::uint64 id_;
  // 队员列表
  MemberVector members_;

  // 队伍最大人数
  static const size_t kMaxSize_ = 5;
};

template <typename T>
void Team::BroadcastMessage(const T &message, gateway::protocol::MessageType::type type) {
  global::ThriftPacket *thrift =
    SessionServerSingleton::GetInstance().GetThrift();
  if(thrift == NULL) {
    LOG_ERROR("%s:%d (%s) Get thrift packet is null.",
        __FILE__, __LINE__, __FUNCTION__);
    return ;
  }
  MemberVector::iterator iterator = this->members_.begin();
  for(; iterator != this->members_.end(); ++iterator) {
    if(*iterator) {
      size_t size = 0;
      const char *message1 = thrift->Serialize(message, size);
      if(message1 == NULL) {
        LOG_ERROR("%s:%d (%s) Serialize message failed.",
            __FILE__, __LINE__, __FUNCTION__);
        return ;
      }
      (*iterator)->SendMessage(type, message1, size);
    }
  }
}

}  // namespace team

}  // namespace server

}  // namespace session

#endif  // __SESSION__SERVER__TEAM__TEAM__H


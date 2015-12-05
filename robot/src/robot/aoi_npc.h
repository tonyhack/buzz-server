#ifndef BUZZ_ROBOT_ROBOT_AOI_NPC_H
#define BUZZ_ROBOT_ROBOT_AOI_NPC_H

#include <stdint.h>
#include <vector>

#include "robot/aoi_role.h"

namespace gateway {
namespace protocol {

class AoiNpcData;
class MessageCreateNpcSynchronize;
class MessageSynchronizeNpc;

} // namespace protocol
} // namespace gateway

namespace robot {

class AoiNpc : public AoiRole {
public:
    AoiNpc(const gateway::protocol::AoiNpcData &data);
    AoiNpc(const gateway::protocol::MessageCreateNpcSynchronize &data);
    virtual ~AoiNpc();

    using AoiRole::update;
    void update(const gateway::protocol::MessageSynchronizeNpc &data);

    int32_t getTemplateId() const { return template_id_; } 

private:
    int32_t template_id_;
    std::vector<int32_t> npc_aoi_attrs_;
};

} // namespace robot

#endif


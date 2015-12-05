#ifndef BUZZ_ROBOT_ROBOT_AOI_ACTOR_H
#define BUZZ_ROBOT_ROBOT_AOI_ACTOR_H

#include <stdint.h>
#include <vector>

#include "robot/aoi_role.h"

namespace gateway {
namespace protocol {

class AoiActorData; 
class MessageCreateActorSynchronize;
class MessageSynchronizeActor;
class MessageActorRelocation;

} // namespace protocol
} // namespace gateway

namespace robot {

class AoiActor : public AoiRole {
public:
    AoiActor(const gateway::protocol::AoiActorData &data);
    AoiActor(const gateway::protocol::MessageCreateActorSynchronize &data);
    virtual ~AoiActor();

    using AoiRole::update;
    void update(const gateway::protocol::MessageSynchronizeActor &data);
    void update(const gateway::protocol::MessageActorRelocation &data);

private:
    std::vector<int32_t> actor_aoi_attrs_;
};

} // namespace robot

#endif


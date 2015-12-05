#ifndef BUZZ_ROBOT_ROBOT_AOI_ROLE_H
#define BUZZ_ROBOT_ROBOT_AOI_ROLE_H

#include <stdint.h>
#include <vector>

#include "robot/aoi_entity.h"

namespace gateway {
namespace protocol {

class MessageRoleMoveSynchronize;

} // namespace protocol
} // namespace gateway

namespace robot {

class AoiRole : public AoiEntity {
public:
    AoiRole() {}
    virtual ~AoiRole() = 0;

    void update(const gateway::protocol::MessageRoleMoveSynchronize &data);
    int getCurrentHp() const;

protected:
    std::vector<int32_t> role_aoi_attrs_;
};

} // namespace robot

#endif


#include "robot/aoi_role.h"

#include "protocol/role_types.h"
#include "protocol/gateway_entity_protocol_types.h"

namespace robot {

AoiRole::~AoiRole()
{
}

void AoiRole::update(const gateway::protocol::MessageRoleMoveSynchronize &data)
{
    pos_.moveByDirection(data.dir_);
}

int AoiRole::getCurrentHp() const
{
    return role_aoi_attrs_[entity::RoleAoiFields::CURRENT_HP];
}

} // namespace robot


#include "robot/aoi_actor.h"

#include <cstdlib>

#include "protocol/gateway_entity_protocol_types.h"

namespace robot {

AoiActor::AoiActor(const gateway::protocol::AoiActorData &data)
{
    id_ = ::atol(data.id_.c_str());
    pos_.x_ = data.pos_x_;
    pos_.y_ = data.pos_y_;
    type_ = entity::EntityType::TYPE_ACTOR;
    role_aoi_attrs_ = data.role_aoi_fields_;
    actor_aoi_attrs_ = data.actor_aoi_fields_;
}

AoiActor::AoiActor(const gateway::protocol::MessageCreateActorSynchronize &data)
{
    id_ = ::atol(data.id_.c_str());
    pos_.x_ = data.pos_x_;
    pos_.y_ = data.pos_y_;
    type_ = entity::EntityType::TYPE_ACTOR;
    role_aoi_attrs_ = data.role_aoi_fields_;
    actor_aoi_attrs_ = data.actor_aoi_fields_;
}

AoiActor::~AoiActor()
{
}

void AoiActor::update(const gateway::protocol::MessageSynchronizeActor &data)
{
    if (data.__isset.role_aoi_fields_) {
        for (size_t i = 0; i < data.role_aoi_fields_.size(); ++i) {
            int field = data.role_aoi_fields_[i].field_;
            int value = data.role_aoi_fields_[i].value_;

            if (field >= 0 && field < (int)role_aoi_attrs_.size()) {
                role_aoi_attrs_[field] = value;
            }
        }
    }
    if (data.__isset.actor_aoi_fields_) {
        for (size_t i = 0; i < data.actor_aoi_fields_.size(); ++i) {
            int field = data.actor_aoi_fields_[i].field_;
            int value = data.actor_aoi_fields_[i].value_;

            if (field >= 0 && field < (int)actor_aoi_attrs_.size()) {
                actor_aoi_attrs_[field] = value;
            }
        }
    }
}

void AoiActor::update(const gateway::protocol::MessageActorRelocation &data)
{
    pos_.x_ = data.x_;
    pos_.y_ = data.y_;
}

} // namespace robot


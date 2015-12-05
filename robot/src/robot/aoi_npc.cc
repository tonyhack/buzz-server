#include "robot/aoi_npc.h"

#include "protocol/gateway_entity_protocol_types.h"

namespace robot {

AoiNpc::AoiNpc(const gateway::protocol::AoiNpcData &data)
{
    id_ = ::atol(data.id_.c_str());
    pos_.x_ = data.pos_x_;
    pos_.y_ = data.pos_y_;
    type_ = entity::EntityType::TYPE_NPC;
    template_id_ = data.template_id_;
    role_aoi_attrs_ = data.role_aoi_fields_;
    npc_aoi_attrs_ = data.npc_aoi_fields_;
}

AoiNpc::AoiNpc(const gateway::protocol::MessageCreateNpcSynchronize &data)
{
    id_ = ::atol(data.id_.c_str());
    pos_.x_ = data.pos_x_;
    pos_.y_ = data.pos_y_;
    type_ = entity::EntityType::TYPE_NPC;
    template_id_ = data.template_id_;
    role_aoi_attrs_ = data.role_aoi_fields_;
    npc_aoi_attrs_ = data.npc_aoi_fields_;
}

AoiNpc::~AoiNpc()
{
}

void AoiNpc::update(const gateway::protocol::MessageSynchronizeNpc &data)
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
    if (data.__isset.npc_aoi_fields_) {
        for (size_t i = 0; i < data.npc_aoi_fields_.size(); ++i) {
            int field = data.npc_aoi_fields_[i].field_;
            int value = data.npc_aoi_fields_[i].value_;

            if (field >= 0 && field < (int)npc_aoi_attrs_.size()) {
                npc_aoi_attrs_[field] = value;
            }
        }
    }
}

} // namespace robot


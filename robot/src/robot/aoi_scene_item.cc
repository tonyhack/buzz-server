#include "robot/aoi_scene_item.h"

#include "protocol/gateway_entity_protocol_types.h"

namespace robot {

AoiSceneItem::AoiSceneItem(const gateway::protocol::AoiSceneItemData &data)
{
    id_ = ::atol(data.id_.c_str());
    pos_.x_ = data.pos_x_;
    pos_.y_ = data.pos_y_;
    type_ = entity::EntityType::TYPE_SCENE_ITEM;
    item_aoi_attrs_ = data.item_aoi_fields_;
}

AoiSceneItem::AoiSceneItem(
    const gateway::protocol::MessageCreateSceneItemSynchronize &data)
{
    id_ = ::atol(data.id_.c_str());
    pos_.x_ = data.pos_x_;
    pos_.y_ = data.pos_y_;
    type_ = entity::EntityType::TYPE_SCENE_ITEM;
    item_aoi_attrs_ = data.scene_item_aoi_fields_;
}

AoiSceneItem::~AoiSceneItem()
{
}

void AoiSceneItem::update(
    const gateway::protocol::MessageSynchronizeSceneItem &data)
{
    if (data.__isset.scene_item_aoi_fields_) {
        for (size_t i = 0; i < data.scene_item_aoi_fields_.size(); ++i) {
            int field = data.scene_item_aoi_fields_[i].field_;
            int value = data.scene_item_aoi_fields_[i].value_;

            if (field >= 0 && field < (int)item_aoi_attrs_.size()) {
                item_aoi_attrs_[field] = value;
            }
        }
    }
}

} // namespace robot


#ifndef BUZZ_ROBOT_ROBOT_AOI_SCENE_ITEM_H
#define BUZZ_ROBOT_ROBOT_AOI_SCENE_ITEM_H

#include <stdint.h>
#include <vector>

#include "robot/aoi_entity.h"

namespace gateway {
namespace protocol {

class AoiSceneItemData;
class MessageCreateSceneItemSynchronize;
class MessageSynchronizeSceneItem;

} // namespace protocol
} // namespace gateway

namespace robot {

class AoiSceneItem : public AoiEntity {
public:
    AoiSceneItem(const gateway::protocol::AoiSceneItemData &data);
    AoiSceneItem(
        const gateway::protocol::MessageCreateSceneItemSynchronize &data);
    virtual ~AoiSceneItem();

    void update(const gateway::protocol::MessageSynchronizeSceneItem &data);

private:
    std::vector<int32_t> item_aoi_attrs_;
};

} // namespace robot

#endif


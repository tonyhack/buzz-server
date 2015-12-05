#ifndef BUZZ_ROBOT_ROBOT_AOI_ENTITY_H
#define BUZZ_ROBOT_ROBOT_AOI_ENTITY_H

#include <stdint.h>

#include "robot/position.h"
#include "protocol/entity_types.h"

namespace robot {

class AoiEntity {
public:
    AoiEntity() {}
    virtual ~AoiEntity() = 0;

    int64_t getId() const { return id_; }
    const Position &getPos() const { return pos_; }
    entity::EntityType::type getType() const { return type_; }
    
protected:
    int64_t id_;
    Position pos_;
    entity::EntityType::type type_;
};

} // namespace robot

#endif


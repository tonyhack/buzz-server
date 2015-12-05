#include "robot/position.h"

#include <cmath>

#include "protocol/constants_constants.h"

namespace robot {

bool Position::operator==(const Position &other) const
{
    return x_ == other.x_ && y_ == other.y_;
}

int Position::getDistance(const Position &target_pos) const
{
    int dx = x_ - target_pos.x_;
    int dy = y_ - target_pos.y_;

    return (int)sqrt(dx * dx + dy * dy);
}

Position::DirectionType Position::getDirection(
    const Position &target_pos) const
{
    int diff_x = target_pos.x_ - x_;
    int diff_y = target_pos.y_ - y_;

    if (0 == diff_x) {
        if (1 == diff_y) {
            return entity::DirectionType::SOUTH;
        } else if (-1 == diff_y) {
            return entity::DirectionType::NORTH;
        }
    } else if (-1 == diff_x) {
        if (0 == diff_y) {
            return entity::DirectionType::WEST;
        } else if (1 == diff_y) {
            return entity::DirectionType::SOUTHWEST;
        } else if (-1 == diff_y) {
            return entity::DirectionType::NORTHWEST;
        }
    } else if (1 == diff_x) {
        if (0 == diff_y) {
            return entity::DirectionType::EAST;
        } else if (1 == diff_y) {
            return entity::DirectionType::SOUTHEAST;
        } else if (-1 == diff_y) {
            return entity::DirectionType::NORTHEAST;
        }
    }

    return entity::DirectionType::NONE;
}

void Position::moveByDirection(DirectionType dir)
{
    if (dir < entity::DirectionType::MIN ||
        dir >= entity::DirectionType::NONE) {
        return;
    }

    x_ += entity::g_constants_constants.kDirectionOffsetX[dir];
    y_ += entity::g_constants_constants.kDirectionOffsetY[dir];
}

} // namespace robot


#ifndef BUZZ_ROBOT_ROBOT_POSITION_H
#define BUZZ_ROBOT_ROBOT_POSITION_H

#include "protocol/direction_types.h"

namespace robot {

class Position {
public:
    typedef entity::DirectionType::type DirectionType;

    Position() : x_(0), y_(0) {}
    Position(int x, int y) : x_(x), y_(y) {}

    bool operator==(const Position &other) const;

    int getDistance(const Position &target_pos) const;
    DirectionType getDirection(const Position &target_pos) const;
    void moveByDirection(DirectionType dir);

public:
    int x_;
    int y_;
};

} // namespace robot

#endif


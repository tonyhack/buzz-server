#ifndef BUZZ_ROBOT_ROBOT_MAP_MANAGER_H
#define BUZZ_ROBOT_ROBOT_MAP_MANAGER_H

#include <string>
#include <vector>
#include <brickred/class_util.h>
#include <brickred/unique_ptr.h>

namespace robot {

class Map;
class Position;

class MapManager {
public:
    bool loadMaps(const std::string &file);
    Map *getMap(int map_id);

private:
    BRICKRED_SINGLETON(MapManager)

    class Impl;
    brickred::UniquePtr<Impl> pimpl_;
};

} // namespace robot

#endif


#ifndef BUZZ_ROBOT_ROBOT_MAP_H
#define BUZZ_ROBOT_ROBOT_MAP_H

#include <string>
#include <vector>
#include <brickred/class_util.h>
#include <brickred/unique_ptr.h>

namespace robot {

class Position;

class Map {
public:
    typedef std::vector<Position> PositionVector;

    Map(int id);
    ~Map();

    bool load(const std::string &info_file,
              const std::string &block_file);

    int getId() const;
    int getHeight() const;
    int getWidth() const;
    int getGridSize() const;
    const std::string &getMapString() const;
    bool isWalkable(const Position &pos);
    void print() const;

    void searchPath(const Position &start_pos,
                    const Position &end_pos,
                    PositionVector *result);
    void printSearchPath(const Position &start_pos,
                         const Position &end_pos);

private:
    BRICKRED_NONCOPYABLE(Map)

    class Impl;
    brickred::UniquePtr<Impl> pimpl_;
};

} // namespace robot

#endif


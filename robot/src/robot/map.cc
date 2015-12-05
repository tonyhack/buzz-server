#include "robot/map.h"

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <tinyxml2/tinyxml2.h>
#include <brickred/string_util.h>

#include "robot/logging.h"
#include "robot/position.h"

namespace robot {

using tinyxml2::XML_NO_ERROR;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;

class Node {
public:
    bool operator<(const Node &other) const { return f_ < other.f_; }
    int neighborGCost(const Node &other) const;
    int heursiticCostEstimate(const Node &other) const;

public:
    Position pos_;
    bool is_walkable_;
    int f_;
    int g_;
    int open_list_pos_;
    int close_list_pos_;
    Node* parent_;
};

///////////////////////////////////////////////////////////////////////////////
int Node::neighborGCost(const Node &other) const
{
    if (pos_.x_ == other.pos_.x_ ||
        pos_.y_ == other.pos_.y_) {
        return 10;
    } else {
        return 14;
    }
}

int Node::heursiticCostEstimate(const Node &other) const
{
    int dx = pos_.x_ - other.pos_.x_;
    int dy = pos_.y_ - other.pos_.y_;

    return (int)(sqrt(dx * dx + dy * dy) * 10);
}

///////////////////////////////////////////////////////////////////////////////
class Map::Impl {
public:
    typedef Map::PositionVector PositionVector;
    typedef std::vector<Node> NodeVector;
    typedef std::vector<Node *> NodePtrVector;

    Impl(int id);
    ~Impl();

    bool load(const std::string &info_file,
              const std::string &block_file);

    int getId() const { return id_; }
    int getHeight() const { return height_; }
    int getWidth() const { return width_; }
    int getGridSize() const { return grid_size_; }
    const std::string &getMapString() const { return map_string_; }
    bool isWalkable(const Position &pos);
    void print() const;

    void searchPath(const Position &start_pos,
                    const Position &end_pos,
                    PositionVector *result);
    void printSearchPath(const Position &start_pos,
                         const Position &end_pos);

public:
    bool loadBlockFile(const std::string &file);

    Node *getNode(const Position &pos);
    Node *getNode(int x, int y);
    void getNeighborNodes(const Node *node, NodePtrVector *neighbor_nodes);

    bool isInOpenList(const Node *node) const;
    bool isOpenListEmpty() const;
    void insertOpenList(Node *node);
    Node *getMinFScoreNodeInOpenList();
    void deleteMinFScoreNodeInOpenList();
    void resortOpenList(Node *node);

    bool isInCloseList(const Node *node) const;
    void insertCloseList(Node *node);

    void constructResultPath(PositionVector *result);

private:
    int id_;
    int height_;
    int width_;
    int grid_size_;
    std::string map_string_;

    NodeVector nodes_;
    Node *start_node_;
    Node *end_node_;
    NodePtrVector open_list_;
};

///////////////////////////////////////////////////////////////////////////////
Map::Impl::Impl(int id) :
    id_(id), height_(0), width_(0), grid_size_(0),
    start_node_(NULL), end_node_(NULL)
{
}

Map::Impl::~Impl()
{
}

bool Map::Impl::load(const std::string &info_file,
                     const std::string &block_file)
{
    if (loadBlockFile(block_file) == false) {
        LOG_ERROR("load block file(%s) failed", block_file.c_str());
        return false;
    }

    return true;
}

bool Map::Impl::loadBlockFile(const std::string &file)
{
    XMLDocument doc;
    if (doc.LoadFile(file.c_str()) != XML_NO_ERROR) {
        LOG_ERROR("load config file(%s) failed", file.c_str());
        return false;
    }

    // map node
    XMLElement *map_node = doc.FirstChildElement("map");
    if (NULL == map_node) {
        LOG_ERROR("/map not found");
        return false;
    }

    const char *attr = NULL;

    attr = map_node->Attribute("gridSize");
    if (NULL == attr) {
        LOG_ERROR("/map[gridSize] not found");
        return false;
    }
    grid_size_ = ::atoi(attr);

    // check grid_size
    if (grid_size_ <= 0) {
        LOG_ERROR("/map[gridSize] is invalid");
        return false;
    }

    attr = map_node->Attribute("height");
    if (NULL == attr) {
        LOG_ERROR("/map[height] not found");
        return false;
    }
    height_ = ::atoi(attr) / grid_size_;

    attr = map_node->Attribute("width");
    if (NULL == attr) {
        LOG_ERROR("/map[width] not found");
        return false;
    }
    width_ = ::atoi(attr) / grid_size_;

    map_string_.resize(height_ * width_, '0');

    // grids node
    XMLElement *grids_node = map_node->FirstChildElement("grids");
    if (NULL == grids_node) {
        LOG_ERROR("/map/grids not found");
        return false;
    }

    for (XMLElement *grid_node = grids_node->FirstChildElement("grid");
         grid_node != NULL;
         grid_node = grid_node->NextSiblingElement("grid")) {
        attr = grid_node->Attribute("attribute");
        if (NULL == attr) {
            LOG_ERROR("/map/grids/grid[attribute] not found");
            return false;
        }
        int attribute = ::atoi(attr);
        if (attribute != 1) {
            continue;
        }

        attr = grid_node->Attribute("contain");
        if (NULL == attr) {
            LOG_ERROR("/map/grids/grid[contain] not found");
            return false;
        }

        std::vector<std::string> positions;
        brickred::string_util::split(attr, ",", &positions);

        for (size_t i = 0; i < positions.size(); ++i) {
            std::vector<std::string> coords;
            brickred::string_util::split(positions[i].c_str(), "_", &coords);
            if (coords.size() != 2) {
                LOG_ERROR("invalid coord(%s)", positions[i].c_str());
                return false;
            }

            int x = ::atoi(coords[0].c_str());
            int y = ::atoi(coords[1].c_str());

            if (x < 0 || x >= width_ ||
                y < 0 || y >= height_) {
                continue;
            }

            map_string_[y * width_ + x] = '1';
        }
    }

    // insert nodes
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            Node node;
            node.pos_.x_ = x;
            node.pos_.y_ = y;
            node.is_walkable_ = (map_string_[y * width_ + x] == '0');
            nodes_.push_back(node);
        }
    }

    return true;
}

bool Map::Impl::isWalkable(const Position &pos)
{
    if (pos.x_ < 0 || pos.x_ >= width_ ||
        pos.y_ < 0 || pos.y_ >= height_) {
        return false;
    }

    return getNode(pos)->is_walkable_;
}

void Map::Impl::print() const
{
    for (int i = 0; i < height_; ++i) {
        ::printf("%*.*s\n", width_, width_, map_string_.c_str() + i * width_);
    }
}

void Map::Impl::searchPath(const Position &start_pos,
                           const Position &end_pos,
                           PositionVector *result)
{
    if (start_pos.x_ < 0 || start_pos.x_ >= width_ ||
        start_pos.y_ < 0 || start_pos.y_ >= height_) {
        LOG_ERROR("start position(%d, %d) is invalid",
                  start_pos.x_, start_pos.y_);
        return;
    }
    if (end_pos.x_ < 0 || end_pos.x_ >= width_ || 
        end_pos.y_ < 0 || end_pos.y_ >= height_) {
        LOG_ERROR("end position(%d, %d) is invalid",
                  end_pos.x_, end_pos.y_);
        return;
    }

    // init data structure
    for (size_t i = 0; i < nodes_.size(); ++i) {
        Node *node = &nodes_[i];
        node->f_ = 0;
        node->g_ = 0;
        node->open_list_pos_ = 0;
        node->close_list_pos_ = 0;
        node->parent_ = NULL;
    }
    start_node_ = getNode(start_pos);
    end_node_ = getNode(end_pos);
    open_list_.clear();
    open_list_.push_back(NULL);

    // search start
    start_node_->g_ = 0;
    start_node_->f_ = start_node_->g_ +
                      start_node_->heursiticCostEstimate(*end_node_);
    insertOpenList(start_node_);

    NodePtrVector neighbor_nodes;
    neighbor_nodes.reserve(8);

    while (!isOpenListEmpty()) {
        Node *cur_node = getMinFScoreNodeInOpenList();
        if (cur_node == end_node_) {
            constructResultPath(result);
            return;
        }

        deleteMinFScoreNodeInOpenList();
        insertCloseList(cur_node);

        getNeighborNodes(cur_node, &neighbor_nodes);

        for (size_t i = 0; i < neighbor_nodes.size(); ++i) {
            Node *neighbor = neighbor_nodes[i];

            bool is_in_open_list = isInOpenList(neighbor);
            bool is_in_close_list = isInCloseList(neighbor);
            int g_cal = cur_node->g_ +
                        cur_node->neighborGCost(*neighbor);

            if (is_in_close_list && g_cal >= neighbor->g_) {
                continue;
            }

            if (!is_in_open_list || g_cal < neighbor->g_) {
                neighbor->parent_ = cur_node;
                neighbor->g_ = g_cal;
                neighbor->f_ = neighbor->g_ +
                               neighbor->heursiticCostEstimate(*end_node_);

                if (!is_in_open_list) {
                    insertOpenList(neighbor);
                } else {
                    resortOpenList(neighbor);
                }
            }
        }
    }
}

Node *Map::Impl::getNode(const Position &pos)
{
    return &nodes_[pos.y_ * width_ + pos.x_];
}

Node *Map::Impl::getNode(int x, int y)
{
    return &nodes_[y * width_ + x];
}

void Map::Impl::getNeighborNodes(const Node *node,
                                 NodePtrVector *neighbor_nodes)
{
    neighbor_nodes->clear();

    int start_x = std::max(node->pos_.x_ - 1, 0);
    int end_x = std::min(node->pos_.x_ + 1, width_ - 1);
    int start_y = std::max(node->pos_.y_ - 1, 0);
    int end_y = std::min(node->pos_.y_ + 1, height_ - 1);

    for (int y = start_y; y <= end_y; ++y) {
        for (int x = start_x; x <= end_x; ++x) {
            if (!(x == node->pos_.x_ && y == node->pos_.y_)) {
                Node* neighbor = getNode(x, y);
                if (neighbor->is_walkable_) {
                    neighbor_nodes->push_back(neighbor);
                }
            }
        }
    }
}

bool Map::Impl::isInOpenList(const Node *node) const
{
    return node->open_list_pos_;
}

bool Map::Impl::isOpenListEmpty() const
{
    return open_list_.size() <= 1;
}

void Map::Impl::insertOpenList(Node *node)
{
    size_t cur_index = open_list_.size();
    open_list_.push_back(node);
    node->open_list_pos_ = cur_index;

    for (;;) {
        size_t parent_index = cur_index / 2;

        if (0 == parent_index) {
            break;
        }
        if (*open_list_[parent_index] < *open_list_[cur_index]) {
            break;
        }

        open_list_[parent_index]->open_list_pos_ = cur_index;
        open_list_[cur_index]->open_list_pos_ = parent_index;
        std::swap(open_list_[parent_index], open_list_[cur_index]);

        cur_index = parent_index;
    }
}

Node *Map::Impl::getMinFScoreNodeInOpenList()
{
    if (isOpenListEmpty()) {
        return NULL;
    }
    return open_list_[1];
}

void Map::Impl::deleteMinFScoreNodeInOpenList()
{
    if (isOpenListEmpty()) {
        return;
    }

    open_list_[1]->open_list_pos_ = 0;
    open_list_[1] = open_list_.back();
    open_list_.pop_back();

    size_t cur_index = 1;

    for (;;) {
        size_t child_index = cur_index * 2;

        if (child_index >= open_list_.size()) {
            break;
        }

        if (child_index + 1 < open_list_.size() &&
            *open_list_[child_index + 1] < *open_list_[child_index]) {
            ++child_index;
        }

        if (*open_list_[cur_index] < *open_list_[child_index]) {
            break;
        }

        open_list_[cur_index]->open_list_pos_ = child_index;
        open_list_[child_index]->open_list_pos_ = cur_index;
        std::swap(open_list_[cur_index], open_list_[child_index]);

        cur_index = child_index;
    }
}

void Map::Impl::resortOpenList(Node *node)
{
    size_t cur_index = node->open_list_pos_;

    for (;;) {
        size_t parent_index = cur_index / 2;

        if (0 == parent_index) {
            break;
        }
        if (*open_list_[parent_index] < *open_list_[cur_index]) {
            break;
        }

        open_list_[parent_index]->open_list_pos_ = cur_index;
        open_list_[cur_index]->open_list_pos_ = parent_index;
        std::swap(open_list_[parent_index], open_list_[cur_index]);

        cur_index = parent_index;
    }
}

bool Map::Impl::isInCloseList(const Node *node) const
{
    return node->close_list_pos_;
}

void Map::Impl::insertCloseList(Node *node)
{
    node->close_list_pos_ = 1;
}

void Map::Impl::constructResultPath(PositionVector *result)
{
    for (Node *node = end_node_;
        node != start_node_ && node != NULL;
        node = node->parent_) {
        result->push_back(node->pos_);
    }
    std::reverse(result->begin(), result->end());
}

void Map::Impl::printSearchPath(const Position &start_pos,
                                const Position &end_pos)
{
    PositionVector result;
    searchPath(start_pos, end_pos, &result);

    std::string map_string = map_string_;

    for (size_t i = 0; i < result.size(); ++i) {
        map_string[result[i].y_ * width_ + result[i].x_] = 'X';
    }

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            char c = map_string[y * width_ + x];

            if ('X' == c) {
                ::printf("\033[;32m");
                ::printf("X");
                ::printf("\033[0m");
            } else {
                ::printf("%c", c); 
            }
        }
        printf("\n");
    }
}

///////////////////////////////////////////////////////////////////////////////
Map::Map(int id) :
    pimpl_(new Impl(id))
{
}

Map::~Map()
{
}

bool Map::load(const std::string &info_file,
               const std::string &block_file)
{
    return pimpl_->load(info_file, block_file);
}

int Map::getId() const
{
    return pimpl_->getId();
}

int Map::getHeight() const
{
    return pimpl_->getHeight();
}

int Map::getWidth() const
{
    return pimpl_->getWidth();
}

int Map::getGridSize() const
{
    return pimpl_->getGridSize();
}

const std::string &Map::getMapString() const
{
    return pimpl_->getMapString();
}

bool Map::isWalkable(const Position &pos)
{
    return pimpl_->isWalkable(pos);
}

void Map::print() const
{
    pimpl_->print();
}

void Map::searchPath(const Position &start_pos,
                     const Position &end_pos,
                     PositionVector *result)
{
    pimpl_->searchPath(start_pos, end_pos, result);
}

void Map::printSearchPath(const Position &start_pos,
                          const Position &end_pos)
{
    pimpl_->printSearchPath(start_pos, end_pos);
}

} // namespace robot


#ifndef BLK_HPP
#define BLK_HPP

#include <iostream>
#include <vector>
#include <string>
#include <utility> // for std::pair

using namespace std;

enum Direction
{
    N,
    E,
    S,
    W,
    FN,
    FE,
    FS,
    FW
};

class Blk
{
private:
    int id;
    int x, y;
    Direction direction;
    string block_name;
    int through_block_net_num;
    vector<pair<pair<int, int>, int>> through_block_edge_net_num; // to store (X0, Y0), (X1, Y1), 100 as a pair of pairs and an int
    vector<pair<int, int>> block_port_region;
    vector<pair<int, int>> vertics;
    bool is_feedthroughable;
    bool is_tile;

public:
    Blk(int id) : id(id) {}

    int getId() const
    {
        return id;
    }

    void setLocX(int x)
    {
        x = x;
    }

    void setLocY(int y)
    {
        y = y;
    }

    void setDirection(Direction direction)
    {
        this->direction = direction;
    }

    void setBlockName(const string &name)
    {
        block_name = name;
    }

    void setThroughBlockNetNum(int num)
    {
        through_block_net_num = num;
    }

    void addThroughBlockEdgeNetNum(int x0, int y0, int x1, int y1, int value)
    {
        through_block_edge_net_num.push_back(make_pair(make_pair(x0, y0), value));
    }

    void addBlockPortRegion(int x0, int y0, int x1, int y1)
    {
        block_port_region.push_back(make_pair(x0, y0));
        block_port_region.push_back(make_pair(x1, y1));
    }

    void setFeedthroughable(bool feedthroughable)
    {
        is_feedthroughable = feedthroughable;
    }

    void setIsTile(bool is_tile)
    {
        this->is_tile = is_tile;
    }

    void addVertics(int x, int y)
    {
        vertics.push_back(make_pair(x, y));
    }

    int getLocX() const
    {
        return x;
    }

    int getLocY() const
    {
        return y;
    }

    Direction getDirection() const
    {
        return direction;
    }

    string getBlockName() const
    {
        return block_name;
    }

    int getThroughBlockNetNum() const
    {
        return through_block_net_num;
    }

    vector<pair<pair<int, int>, int>> getThroughBlockEdgeNetNum() const
    {
        return through_block_edge_net_num;
    }

    vector<pair<int, int>> getBlockPortRegion() const
    {
        return block_port_region;
    }

    bool isFeedthroughable() const
    {
        return is_feedthroughable;
    }

    bool isTile() const
    {
        return is_tile;
    }

    vector<pair<int, int>> getVertics() const
    {
        return vertics;
    }
};

#endif // BLK_HPP
